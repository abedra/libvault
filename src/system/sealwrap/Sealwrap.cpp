#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Sealwrap::read() {
  return HttpConsumer::get(client_, getUrl());
}

std::optional<std::string> Vault::Sys::Sealwrap::start() {
  return HttpConsumer::post(client_, getUrl(), Parameters{});
}

Vault::Url Vault::Sys::Sealwrap::getUrl() {
  return client_.getUrl("/v1/sys/sealwrap/rewrap", Path{});
}
