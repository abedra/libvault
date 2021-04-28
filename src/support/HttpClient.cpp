#include <utility>
#include "VaultClient.h"
#include "json.hpp"

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
  , responseErrorCallback_([&](const std::string& err){})
{}

Vault::HttpClient::HttpClient(Vault::Config& config,
                              HttpErrorCallback errorCallback,
                              ResponseErrorCallback responseErrorCallback)
  : debug_(config.getDebug())
  , verify_(config.getVerify())
  , connectTimeout_(config.getConnectTimeout().value())
  , caBundle_(config.getCaBundle())
  , errorCallback_(std::move(errorCallback))
  , responseErrorCallback_(std::move(responseErrorCallback))
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
  CURL *curl;
  std::string buffer;
  long response_code = 0;

  curl = curl_easy_init();
  if (curl) {
    struct curl_slist *chunk = nullptr;

    if (!token.empty()) {
      chunk = curl_slist_append(chunk, ("X-Vault-Token: " + token).c_str());
    }

    if (!ns.empty()) {
      chunk = curl_slist_append(chunk, ("X-Vault-Namespace: " + ns).c_str());
    }

    chunk = curl_slist_append(chunk, "Content-Type: application/json");
    chunk = curlHeaderCallback(chunk);

    if (verify_) {
      if (!caBundle_.empty()) {
        curl_easy_setopt(curl, CURLOPT_CAINFO, caBundle_.u8string().c_str());
      }
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
    } else {
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    }

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connectTimeout_);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_URL, url.value().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    if (debug_) {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }

    setupCallback(curl);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      errorCallback_(curl_easy_strerror(res));

      curl_easy_cleanup(curl);
      curl_slist_free_all(chunk);

      return std::nullopt;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(curl);
    curl_slist_free_all(chunk);
  }

  return std::optional<HttpResponse>({
    Vault::HttpResponseStatusCode{response_code},
    Vault::HttpResponseBodyString{buffer}
  });
}

void
Vault::HttpClient::reportError(std::optional<HttpResponse> response) const {
    nlohmann::json j;
    j["Status code"] = response.value().statusCode.value();
    j["Response"] = response.value().body.value();
    responseErrorCallback_(j.dump());
}
