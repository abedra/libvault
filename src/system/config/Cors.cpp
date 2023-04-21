#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Cors::read(const Path &path) {
  return HttpConsumer::get(client_, getUrl());
}

std::optional<std::string>
Vault::Sys::Cors::configure(const Path &path, const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(), parameters);
}

std::optional<std::string> Vault::Sys::Cors::del(const Path &path) {
  return HttpConsumer::del(client_, getUrl());
}

Vault::Url Vault::Sys::Cors::getUrl() {
  return client_.getUrl("/v1/sys/config/cors", Path{});
}
