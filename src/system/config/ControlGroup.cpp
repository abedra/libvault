#include "VaultClient.h"

std::optional<std::string> Vault::Sys::ControlGroup::read(const Path &path) {
  return HttpConsumer::get(client_, getUrl());
}

std::optional<std::string> Vault::Sys::ControlGroup::configure(const Path &path, const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(), parameters);
}

std::optional<std::string> Vault::Sys::ControlGroup::del(const Path &path) {
  return HttpConsumer::del(client_, getUrl());
}

Vault::Url Vault::Sys::ControlGroup::getUrl() {
  return client_.getUrl("/v1/sys/config/control-group", Path{});
}
