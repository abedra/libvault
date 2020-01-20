#include <iostream>
#include "VaultClient.h"

Vault::Sys::Sys(const Vault::Client &client) : client_(client) { }

std::optional<std::string> Vault::Sys::health() {
  return Vault::HttpConsumer::get(client_, getUrl(Vault::Path{"/health"}));
}

std::optional<std::string> Vault::Sys::health(const Vault::Url &leader) {
  return Vault::HttpConsumer::get(client_, Vault::Url{leader + "/v1/sys/health"});
}

std::optional<std::string> Vault::Sys::leader() {
  return Vault::HttpConsumer::get(client_, getUrl(Vault::Path("/leader")));
}

Vault::Url Vault::Sys::getUrl(const Vault::Path &path) {
  return client_.getUrl("/v1/sys", path);
}
