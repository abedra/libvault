#include <iostream>
#include "VaultClient.h"

Sys::Sys(const VaultClient &client) : client_(client) { }

std::optional<std::string> Sys::health() {
  return VaultHttpConsumer::get(client_, getUrl(Vault::Path{"/health"}));
}

std::optional<std::string> Sys::health(const Vault::Url &leader) {
  return VaultHttpConsumer::get(client_, Vault::Url{leader + "/v1/sys/health"});
}

std::optional<std::string> Sys::leader() {
  return VaultHttpConsumer::get(client_, getUrl(Vault::Path("/leader")));
}

Vault::Url Sys::getUrl(const Vault::Path &path) {
  return client_.getUrl("/v1/sys", path);
}
