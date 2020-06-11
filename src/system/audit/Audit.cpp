#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Audit::read() {
  return HttpConsumer::get(client_, getUrl(Path{}));
}

std::optional<std::string> Vault::Sys::Audit::enable(const Path &path, const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(path), parameters);
}

std::optional<std::string> Vault::Sys::Audit::disable(const Path &path) {
  return HttpConsumer::del(client_, getUrl(path));
}

Vault::Url Vault::Sys::Audit::getUrl(const Vault::Path &path) {
  return client_.getUrl("/v1/sys/audit/", path);
}

