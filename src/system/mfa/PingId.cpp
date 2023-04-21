#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Mfa::PingId::read(const Path &path) {
  return Vault::Sys::Mfa::read(client_, getUrl(path));
}

std::optional<std::string>
Vault::Sys::Mfa::PingId::configure(const Path &path,
                                   const Parameters &parameters) {
  return Vault::Sys::Mfa::configure(client_, getUrl(path), parameters);
}

std::optional<std::string>
Vault::Sys::Mfa::PingId::del(const Path &path, const Parameters &parameters) {
  return Vault::Sys::Mfa::del(client_, getUrl(path));
}

Vault::Url Vault::Sys::Mfa::PingId::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/mfa/method/pingid/", path);
}
