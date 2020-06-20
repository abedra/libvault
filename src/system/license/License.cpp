#include "VaultClient.h"

std::optional<std::string> Vault::Sys::License::read() {
  return HttpConsumer::get(client_, getUrl());
}

std::optional<std::string> Vault::Sys::License::install(const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(), parameters);
}

Vault::Url Vault::Sys::License::getUrl() {
  return client_.getUrl("/v1/sys/license", Path{});
}

