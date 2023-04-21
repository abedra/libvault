#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Raw::list(const Path &path) {
  return HttpConsumer::list(client_, getUrl(path));
}

std::optional<std::string> Vault::Sys::Raw::read(const Path &path) {
  return HttpConsumer::get(client_, getUrl(path));
}

std::optional<std::string>
Vault::Sys::Raw::create(const Path &path, const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(path), parameters);
}

std::optional<std::string>
Vault::Sys::Raw::update(const Path &path, const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(path), parameters);
}

std::optional<std::string> Vault::Sys::Raw::del(const Path &path) {
  return HttpConsumer::del(client_, getUrl(path));
}

Vault::Url Vault::Sys::Raw::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/raw/", path);
}
