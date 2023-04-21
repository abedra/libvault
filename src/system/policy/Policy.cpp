#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Policy::list() const {
  return HttpConsumer::get(client_, getUrl(Path{}));
}

std::optional<std::string> Vault::Sys::Policy::read(const Path &path) const {
  return HttpConsumer::get(client_, getUrl(path));
}

std::optional<std::string>
Vault::Sys::Policy::create(const Path &path,
                           const Parameters &parameters) const {
  return HttpConsumer::put(client_, getUrl(path), parameters);
}

std::optional<std::string>
Vault::Sys::Policy::update(const Path &path,
                           const Parameters &parameters) const {
  return HttpConsumer::put(client_, getUrl(path), parameters);
}

std::optional<std::string> Vault::Sys::Policy::del(const Path &path) const {
  return HttpConsumer::del(client_, getUrl(path));
}

Vault::Url Vault::Sys::Policy::getUrl(const Path &path) const {
  return client_.getUrl("/v1/sys/policy/", path);
}
