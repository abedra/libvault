#include "VaultClient.h"

VaultClient::VaultClient(VaultConfig& config,
			 AuthenticationStrategy& authStrategy) :
  host_(config.getHost()),
  port_(config.getPort()),
  namespace_(config.getNamespace()),
  tls_(config.getTls()),
  authStrategy_(authStrategy),
  httpClient_(HttpClient(config))
{
  auto result = authStrategy_.authenticate(*this);
  if (result) {
    token_ = result.value();
  }
}

VaultClient::VaultClient(VaultConfig& config,
			 AuthenticationStrategy& authStrategy,
			 HttpErrorCallback httpErrorCallback) :
  host_(config.getHost()),
  port_(config.getPort()),
  namespace_(config.getNamespace()),
  tls_(config.getTls()),
  authStrategy_(authStrategy),
  httpClient_(HttpClient(config, httpErrorCallback))
{
  auto result = authStrategy_.authenticate(*this);
  if (result) {
    token_ = result.value();
  }
}

std::string VaultClient::getUrl(std::string base, std::string path) const {
  return (tls_ ? "https://" : "http://") + host_ + ":" + port_ + base + path;
}
