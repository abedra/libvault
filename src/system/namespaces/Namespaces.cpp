#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Namespaces::list() {
  return HttpConsumer::list(client_, getUrl(Path{}));
}

std::optional<std::string> Vault::Sys::Namespaces::read(const Path &path) {
  return HttpConsumer::get(client_, getUrl(path));
}

std::optional<std::string>
Vault::Sys::Namespaces::create(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(path), parameters);
}

std::optional<std::string> Vault::Sys::Namespaces::del(const Path &path) {
  return HttpConsumer::del(client_, getUrl(path));
}

Vault::Url Vault::Sys::Namespaces::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/namespaces/", path);
}
