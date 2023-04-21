#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Mfa::Okta::read(const Path &path) {
  return Vault::Sys::Mfa::read(client_, getUrl(path));
}

std::optional<std::string>
Vault::Sys::Mfa::Okta::configure(const Path &path,
                                 const Parameters &parameters) {
  return Vault::Sys::Mfa::configure(client_, getUrl(path), parameters);
}

std::optional<std::string>
Vault::Sys::Mfa::Okta::del(const Path &path, const Parameters &parameters) {
  return Vault::Sys::Mfa::del(client_, getUrl(path));
}

Vault::Url Vault::Sys::Mfa::Okta::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/mfa/method/okta/", path);
}
