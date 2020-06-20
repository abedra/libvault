#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Init::read() {
  return HttpConsumer::get(client_, getUrl());
}

std::optional<std::string> Vault::Sys::Init::start(const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(), parameters);
}

Vault::Url Vault::Sys::Init::getUrl() {
  return client_.getUrl("/v1/sys/init", Path{});
}
