#include "VaultClient.h"
#include <utility>

Vault::HttpClient::HttpClient(Vault::Config config)
    : config_(std::move(config)),
      errorCallback_([&](const std::string &err) {}),
      responseErrorCallback_([&](const HttpResponse &err) {}) {}

Vault::HttpClient::HttpClient(Vault::Config config,
                              HttpErrorCallback errorCallback,
                              ResponseErrorCallback responseErrorCallback)
    : config_(std::move(config)), errorCallback_(std::move(errorCallback)),
      responseErrorCallback_(std::move(responseErrorCallback)) {}

bool Vault::HttpClient::is_success(std::optional<HttpResponse> response) {
  return response && (response.value().statusCode.value() >= 200 &&
                      response.value().statusCode.value() < 300);
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::get(const Vault::Url &url, const Vault::Token &token,
                       const Vault::Namespace &ns) const {
  return executeRequest(
      url, token, ns, [&](CURL *curl) {},
      [&](curl_slist *chunk) { return chunk; }, errorCallback_);
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::post(const Vault::Url &url, const Vault::Token &token,
                        const Vault::Namespace &ns,
                        const std::string &value) const {
  return executeRequest(
      url, token, ns,
      [&](CURL *curl) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
      },
      [&](curl_slist *chunk) { return chunk; }, errorCallback_);
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::post(const Vault::Url &url, const Vault::Token &token,
                        const Vault::Namespace &ns, const std::string &value,
                        const Vault::CurlHeaderCallback &headerCallback) const {
  return executeRequest(
      url, token, ns,
      [&](CURL *curl) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
      },
      headerCallback, errorCallback_);
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::post(const Vault::Url &url, const Vault::Certificate &cert,
                        const Vault::Certificate &key,
                        const Vault::Namespace &ns) const {
  return executeRequest(
      url, Token{}, ns,
      [&](CURL *curl) {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
        curl_easy_setopt(curl, CURLOPT_SSLCERT, cert.value().c_str());
        curl_easy_setopt(curl, CURLOPT_SSLKEY, key.value().c_str());
      },
      [&](curl_slist *chunk) { return chunk; }, errorCallback_);
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::put(const Vault::Url &url, const Vault::Token &token,
                       const Vault::Namespace &ns,
                       const std::string &value) const {
  return executeRequest(
      url, token, ns,
      [&](CURL *curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, value.c_str());
      },
      [&](curl_slist *chunk) { return chunk; }, errorCallback_);
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::del(const Vault::Url &url, const Vault::Token &token,
                       const Vault::Namespace &ns) const {
  return executeRequest(
      url, token, ns,
      [&](CURL *curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
      },
      [&](curl_slist *chunk) { return chunk; }, errorCallback_);
}

std::optional<Vault::HttpResponse>
Vault::HttpClient::list(const Vault::Url &url, const Vault::Token &token,
                        const Vault::Namespace &ns) const {
  return executeRequest(
      url, token, ns,
      [&](CURL *curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "LIST");
      },
      [&](curl_slist *chunk) { return chunk; }, errorCallback_);
}

void Vault::HttpClient::handleResponseError(
    const HttpResponse &response) const {
  return responseErrorCallback_(response);
}

std::optional<Vault::HttpResponse> Vault::HttpClient::executeRequest(
    const Vault::Url &url, const Vault::Token &token,
    const Vault::Namespace &ns, const Vault::CurlSetupCallback &setupCallback,
    const Vault::CurlHeaderCallback &curlHeaderCallback,
    const Vault::HttpErrorCallback &errorCallback) const {
  CurlWrapper curlWrapper{errorCallback};

  if (!token.empty()) {
    curlWrapper.appendHeader("X-Vault-Token: " + token);
  }

  if (!ns.empty()) {
    curlWrapper.appendHeader("X-Vault-Namespace: " + ns);
  }

  curlWrapper.appendHeader("Content-Type: application/json");
  curlWrapper.setupHeaders(curlHeaderCallback);

  if (config_.getVerify()) {
    if (!config_.getCaBundle().empty()) {
      curlWrapper.setOption(CURLOPT_CAINFO,
                            config_.getCaBundle().u8string().c_str());
    }

    curlWrapper.setOption(CURLOPT_SSL_VERIFYPEER, 1);
  } else {
    curlWrapper.setOption(CURLOPT_SSL_VERIFYPEER, 0);
  }

  curlWrapper.setOption(CURLOPT_CONNECTTIMEOUT,
                        config_.getConnectTimeout().value());
  curlWrapper.setOption(CURLOPT_TIMEOUT, config_.getRequestTimeout().value());
  curlWrapper.setOption(CURLOPT_LOW_SPEED_TIME,
                        config_.getLowSpeedTimeout().value());
  curlWrapper.setOption(CURLOPT_LOW_SPEED_LIMIT,
                        config_.getLowSpeedLimit().value());
  curlWrapper.setOption(CURLOPT_URL, url.value().c_str());

  if (config_.getDebug()) {
    curlWrapper.setOption(CURLOPT_VERBOSE, 1L);
  }

  curlWrapper.setupOptions(setupCallback);

  return curlWrapper.execute();
}
