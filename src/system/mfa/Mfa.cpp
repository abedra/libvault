#include "VaultClient.h"

std::optional<std::string>
Vault::Sys::Mfa::configure(const Client &client, const Url &url, const Parameters &parameters) {
  return HttpConsumer::post(client, url, parameters);
}

std::optional<std::string> Vault::Sys::Mfa::read(const Client &client, const Url &url) {
  return HttpConsumer::get(client, url);
}

std::optional<std::string> Vault::Sys::Mfa::del(const Client &client, const Url &url) {
  return HttpConsumer::del(client, url);
}
