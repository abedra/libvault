#include <utility>
#include "VaultClient.h"

static size_t
writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *) userp)->append((char *) contents, size * nmemb);
  return size * nmemb;
}

Vault::HttpClient::HttpClient(Vault::Config& config)
  : debug_(config.getDebug())
  , verify_(config.getVerify())
  , connectTimeout_(config.getConnectTimeout().value())
  , caBundle_(config.getCaBundle())
  , errorCallback_([&](const std::string& err){})
  , responseErrorCallback([&](const HttpResponse& err){})
{}

Vault::HttpClient::HttpClient(Vault::Config& config,
                              HttpErrorCallback errorCallback,
                              ResponseErrorCallback responseErrorCallback)
  : debug_(config.getDebug())
  , verify_(config.getVerify())
  , connectTimeout_(config.getConnectTimeout().value())
  , caBundle_(config.getCaBundle())
  , errorCallback_(std::move(errorCallback))
  , responseErrorCallback(std::move(responseErrorCallback))
{}

bool Vault::HttpClient::is_success(std::optional<HttpResponse> response) {
  return response &&
    (response.value().statusCode.value() >= 200 && response.value().statusCode.value() < 300);
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::get(const Vault::Url& url,
                       const Vault::Token& token,
                       const Vault::Namespace& ns) const {
  return executeRequest(
    url,
    token,
    ns,
    [&](CURL *curl) {},
    [&](curl_slist *chunk){ return chunk; },
    errorCallback_
  );
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::post(const Vault::Url& url,
                        const Vault::Token& token,
                        const Vault::Namespace& ns,
                        const std::string& value) const {
  return executeRequest(
    url,
    token,
    ns,
    [&](CURL *curl) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
    },
    [&](curl_slist *chunk){ return chunk; },
    errorCallback_
  );
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::post(const Vault::Url& url,
                        const Vault::Token& token,
                        const Vault::Namespace& ns,
                        const std::string& value,
                        const Vault::CurlHeaderCallback& headerCallback) const {
  return executeRequest(
    url,
    token,
    ns,
    [&](CURL *curl) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
    },
    headerCallback,
    errorCallback_
  );
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::post(const Vault::Url& url,
                        const Vault::Certificate &cert,
                        const Vault::Certificate &key,
                        const Vault::Namespace& ns) const {
  return executeRequest(
      url,
      Token{},
      ns,
      [&](CURL *curl) {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
        curl_easy_setopt(curl, CURLOPT_SSLCERT, cert.value().c_str());
        curl_easy_setopt(curl, CURLOPT_SSLKEY, key.value().c_str());
      },
      [&](curl_slist *chunk){ return chunk; },
      errorCallback_
  );
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::put(const Vault::Url& url,
                       const Vault::Token& token,
                       const Vault::Namespace& ns,
                       const std::string& value) const {
  return executeRequest(
    url,
    token,
    ns,
    [&](CURL *curl) {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
    },
    [&](curl_slist *chunk){ return chunk; },
    errorCallback_
  );
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::del(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns) const {
  return executeRequest(
    url,
    token,
    ns,
    [&](CURL *curl) {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    },
    [&](curl_slist *chunk){ return chunk; },
    errorCallback_
  );
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::list(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns) const {
  return executeRequest(
    url,
    token,
    ns,
    [&](CURL *curl) {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "LIST");
    },
    [&](curl_slist *chunk){ return chunk; },
    errorCallback_
  );
}

struct CurlWrapper final {
    explicit CurlWrapper(const Vault::HttpErrorCallback& errorCallback) {
        curl_ = curl_easy_init();
        slist_ = nullptr;
        errorCallback_ = errorCallback;
    }

    ~CurlWrapper() {
        curl_easy_cleanup(curl_);
        curl_slist_free_all(slist_);
    }

    void appendHeader(const std::string& header) {
        curl_slist_append(slist_, header.c_str());
    }

    void setupHeaders(const Vault::CurlHeaderCallback &curlHeaderCallback) {
        slist_ = curlHeaderCallback(slist_);
    }

    void setupOptions(const Vault::CurlSetupCallback &curlSetupCallback) {
        curlSetupCallback(curl_);
    }

    template<class A>
    void setOption(const CURLoption option, A value) {
        curl_easy_setopt(curl_, option, value);
    }

    [[nodiscard]]
    std::optional<Vault::HttpResponse> execute() const {
        std::string buffer;

        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, slist_);

        CURLcode res = curl_easy_perform(curl_);
        if (res != CURLE_OK) {
            errorCallback_(curl_easy_strerror(res));
            return std::nullopt;
        }

        long responseCode = 0;
        curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, responseCode);

        return std::optional<Vault::HttpResponse>({
            Vault::HttpResponseStatusCode{responseCode},
            Vault::HttpResponseBodyString{buffer}
        });
    }

private:
    CURL *curl_;
    curl_slist *slist_;
    Vault::HttpErrorCallback errorCallback_;
};

std::optional<Vault::HttpResponse>
Vault::HttpClient::executeRequest(const Vault::Url& url,
                                  const Vault::Token& token,
                                  const Vault::Namespace& ns,
                                  const Vault::CurlSetupCallback& setupCallback,
                                  const Vault::CurlHeaderCallback& curlHeaderCallback,
                                  const Vault::HttpErrorCallback& errorCallback) const {
    auto curlWrapper = std::make_unique<CurlWrapper>(errorCallback);

    if (!token.empty()) {
        curlWrapper->appendHeader("X-Vault-Token: " + token);
    }

    if (!ns.empty()) {
        curlWrapper->appendHeader("X-Vault-Namespace: " + ns);
    }

    curlWrapper->appendHeader("Content-Type: application/json");
    curlWrapper->setupHeaders(curlHeaderCallback);

    if (verify_) {
        if (!caBundle_.empty()) {
            curlWrapper->setOption(CURLOPT_CAINFO, caBundle_.u8string().c_str());
        }

        curlWrapper->setOption(CURLOPT_SSL_VERIFYPEER, 1);
    } else {
        curlWrapper->setOption(CURLOPT_SSL_VERIFYPEER, 0);
    }

    curlWrapper->setOption(CURLOPT_CONNECTTIMEOUT, connectTimeout_);
    curlWrapper->setOption(CURLOPT_URL, url.value().c_str());

    if (debug_) {
        curlWrapper->setOption(CURLOPT_VERBOSE, 1L);
    }

    curlWrapper->setupOptions(setupCallback);

    return curlWrapper->execute();
}
