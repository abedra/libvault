#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Auditing::list() {
  return HttpConsumer::get(client_, getUrl(Path{"request-headers"}));
}

std::optional<std::string> Vault::Sys::Auditing::read(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"request-headers/" + path}));
}

std::optional<std::string>
Vault::Sys::Auditing::create(const Path &path, const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"request-headers/" + path}),
                           parameters);
}

std::optional<std::string>
Vault::Sys::Auditing::update(const Path &path, const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"request-headers/" + path}),
                           parameters);
}

std::optional<std::string> Vault::Sys::Auditing::del(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"request-headers/" + path}));
}

Vault::Url Vault::Sys::Auditing::getUrl(const Vault::Path &path) {
  return client_.getUrl("/v1/sys/config/auditing", path);
}
