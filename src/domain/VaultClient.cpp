#include <utility>
#include "VaultClient.h"

Vault::Client::Client(Vault::Config& config, AuthenticationStrategy& authStrategy)
  : host_(config.getHost())
  , port_(config.getPort())
  , namespace_(config.getNamespace())
  , tls_(config.getTls())
  , authStrategy_(authStrategy)
  , httpClient_(HttpClient(config))
  , debug_(config.getDebug())
  , caBundle_(config.getCaBundle())
{
  if (auto result = authStrategy_.authenticate(*this)) {
    token_ = result.value().token;
  }
}

Vault::Client::Client(Vault::Config& config,
                      AuthenticationStrategy& authStrategy,
                      HttpErrorCallback httpErrorCallback)
  : host_(config.getHost())
  , port_(config.getPort())
  , namespace_(config.getNamespace())
  , tls_(config.getTls())
  , authStrategy_(authStrategy)
  , httpClient_(HttpClient(config, std::move(httpErrorCallback)))
  , debug_(config.getDebug())
  , caBundle_(config.getCaBundle())
{
  if (auto result = authStrategy_.authenticate(*this)) {
    token_ = result.value().token;
  }
}

Vault::Client::Client(const Vault::Client& other, Vault::Token token)
  : host_(other.getHost())
  , port_(other.getPort())
  , namespace_(other.getNamespace())
  , tls_(other.getTls())
  , authStrategy_(other.getAuthenticationStrategy())
  , httpClient_(other.getHttpClient())
  , debug_(other.getDebug())
  , token_(std::move(token))
  , caBundle_(other.getCaBundle())
{}

Vault::Url Vault::Client::getUrl(const std::string& base, const Vault::Path& path) const {
  return Vault::Url{(tls_ ? "https://" : "http://") + host_ + ":" + port_ + base + path};
}
