#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Plugins::list() {
  return HttpConsumer::get(client_, getUrl(Path{}));
}

std::optional<std::string> Vault::Sys::Plugins::list(const Path &path) {
  return HttpConsumer::get(client_, getUrl(path));
}

std::optional<std::string>
Vault::Sys::Plugins::reg(const Path &type, const Path &name, const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{type + "/" + name}), parameters);
}

std::optional<std::string> Vault::Sys::Plugins::read(const Path &type, const Path &name) {
  return HttpConsumer::get(client_, getUrl(Path{type + "/" + name}));
}

std::optional<std::string> Vault::Sys::Plugins::del(const Path &type, const Path &name) {
  return HttpConsumer::del(client_, getUrl(Path{type + "/" + name}));
}

Vault::Url Vault::Sys::Plugins::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/plugins/catalog/", path);
}

