#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Mounts::readUi() {
  return HttpConsumer::get(client_, getUrl(Path{"internal/ui/mounts"}));
}

std::optional<std::string> Vault::Sys::Mounts::read() {
  return HttpConsumer::get(client_, getUrl(Path{"mounts"}));
}

std::optional<std::string> Vault::Sys::Mounts::enable(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"mounts/" + path}), parameters);
}

Vault::Url Vault::Sys::Mounts::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/", path);
}

