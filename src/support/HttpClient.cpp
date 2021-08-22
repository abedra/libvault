#include <memory>
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
  , responseErrorCallback([&](HttpResponse err){})
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
    [&](curl_slist *chunk){ return chunk; }
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
    [&](curl_slist *chunk){ return chunk; }
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
    headerCallback
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
      [&](curl_slist *chunk){ return chunk; }
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
    [&](curl_slist *chunk){ return chunk; }
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
    [&](curl_slist *chunk){ return chunk; }
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
    [&](curl_slist *chunk){ return chunk; }
  );
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::executeRequest(const Vault::Url& url,
                                  const Vault::Token& token,
                                  const Vault::Namespace& ns,
                                  const Vault::CurlSetupCallback& setupCallback,
                                  const Vault::CurlHeaderCallback& curlHeaderCallback) const {
  struct CurlEasyDeleter {
    void operator() (CURL *curl) const noexcept {
      curl_easy_cleanup(curl);
    }
  };

  std::unique_ptr<CURL, CurlEasyDeleter> curl;
  std::string buffer;
  long response_code = 0;

  curl.reset(curl_easy_init());
  if (curl) {
    struct SlistDeleter {
      void operator() (struct curl_slist *chunk) const noexcept {
        curl_slist_free_all(chunk);
      }
    };

    std::unique_ptr<struct curl_slist, SlistDeleter> chunk;

    if (!token.empty()) {
      chunk.reset(curl_slist_append(chunk.release(), ("X-Vault-Token: " + token).c_str()));
    }

    if (!ns.empty()) {
      chunk.reset(curl_slist_append(chunk.release(), ("X-Vault-Namespace: " + ns).c_str()));
    }

    chunk.reset(curl_slist_append(chunk.release(), "Content-Type: application/json"));
    chunk.reset(curlHeaderCallback(chunk.release()));

    if (verify_) {
      if (!caBundle_.empty()) {
        curl_easy_setopt(curl.get(), CURLOPT_CAINFO, caBundle_.u8string().c_str());
      }
      curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 1);
    } else {
      curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 0);
    }

    curl_easy_setopt(curl.get(), CURLOPT_CONNECTTIMEOUT, connectTimeout_);
    curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, chunk.get());
    curl_easy_setopt(curl.get(), CURLOPT_URL, url.value().c_str());
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &buffer);

    if (debug_) {
      curl_easy_setopt(curl.get(), CURLOPT_VERBOSE, 1L);
    }

    setupCallback(curl.get());

    CURLcode res = curl_easy_perform(curl.get());
    if (res != CURLE_OK) {
      errorCallback_(curl_easy_strerror(res));

      return std::nullopt;
    }

    curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &response_code);
  }

  return std::optional<HttpResponse>({
    Vault::HttpResponseStatusCode{response_code},
    Vault::HttpResponseBodyString{buffer}
  });
}
