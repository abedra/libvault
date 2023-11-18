#include "VaultClient.h"
#include <utility>

Vault::Client::Client(Vault::Config &config,
                      AuthenticationStrategy &authStrategy)
    : debug_(config.getDebug())
    , tls_(config.getTls())
    , host_(config.getHost())
    , port_(config.getPort())
    , namespace_(config.getNamespace())
    , httpClient_(HttpClient(config))
    , authStrategy_(authStrategy)
    , caBundle_(config.getCaBundle()) {
  if (auto result = authStrategy_.authenticate(*this)) {
    token_ = result.value().token;
  }
}

Vault::Client::Client(Vault::Config &config,
                      AuthenticationStrategy &authStrategy,
                      HttpErrorCallback httpErrorCallback,
                      ResponseErrorCallback responseErrorCallback)
    : debug_(config.getDebug())
    , tls_(config.getTls())
    , host_(config.getHost())
    , port_(config.getPort())
    , namespace_(config.getNamespace())
    , httpClient_(HttpClient(config, std::move(httpErrorCallback),
                             std::move(responseErrorCallback)))
    , authStrategy_(authStrategy)
    , caBundle_(config.getCaBundle()) {
  if (auto result = authStrategy_.authenticate(*this)) {
    token_ = result.value().token;
  }
}

Vault::Client::Client(const Vault::Client &other, Vault::Token token)
    : debug_(other.getDebug())
    , tls_(other.getTls())
    , host_(other.getHost())
    , port_(other.getPort())
    , token_(std::move(token))
    , namespace_(other.getNamespace())
    , httpClient_(other.getHttpClient())
    , authStrategy_(other.getAuthenticationStrategy())
    , caBundle_(other.getCaBundle()) {}

Vault::Url Vault::Client::getUrl(const std::string &base,
                                 const Vault::Path &path) const {
  return Vault::Url{(tls_ ? "https://" : "http://") + host_ + ":" + port_ +
                    base + path};
}
