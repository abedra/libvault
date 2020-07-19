#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Policies::EGP::list() {
  return Vault::Sys::Policies::list(client_, getUrl(Path{}));
}

std::optional<std::string> Vault::Sys::Policies::EGP::read(const Path &path) {
  return Vault::Sys::Policies::read(client_, getUrl(path));
}

std::optional<std::string> Vault::Sys::Policies::EGP::create(const Path &path, const Parameters &parameters) {
  return Vault::Sys::Policies::create(client_, getUrl(path), parameters);
}

std::optional<std::string> Vault::Sys::Policies::EGP::update(const Path &path, const Parameters &parameters) {
  return Vault::Sys::Policies::update(client_, getUrl(path), parameters);
}

std::optional<std::string> Vault::Sys::Policies::EGP::del(const Path &path) {
  return Vault::Sys::Policies::del(client_, getUrl(path));
}

Vault::Url Vault::Sys::Policies::EGP::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/policies/egp/", path);
}
