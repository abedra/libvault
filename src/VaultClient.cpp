#include "VaultClient.h"

#include <utility>

VaultClient::VaultClient(
        VaultConfig& config,
		AuthenticationStrategy& authStrategy)
  : host_(config.getHost())
  , port_(config.getPort())
  , namespace_(config.getNamespace())
  , tls_(config.getTls())
  , authStrategy_(authStrategy)
  , httpClient_(HttpClient(config))
  , debug_(config.getDebug())
{
  auto result = authStrategy_.authenticate(*this);
  if (result) {
    token_ = result.value();
  }
}

VaultClient::VaultClient(
        VaultConfig& config,
		AuthenticationStrategy& authStrategy,
		HttpErrorCallback httpErrorCallback)
  : host_(config.getHost())
  , port_(config.getPort())
  , namespace_(config.getNamespace())
  , tls_(config.getTls())
  , authStrategy_(authStrategy)
  , httpClient_(HttpClient(config, std::move(httpErrorCallback)))
  , debug_(config.getDebug())
{
  auto result = authStrategy_.authenticate(*this);
  if (result) {
    token_ = result.value();
  }
}

std::string VaultClient::getUrl(const std::string& base, const std::string& path) const {
  return (tls_ ? "https://" : "http://") + host_ + ":" + port_ + base + path;
}
