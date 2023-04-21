#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Mfa::Totp::read(const Path &path) {
  return Vault::Sys::Mfa::read(client_, getUrl(path));
}

std::optional<std::string>
Vault::Sys::Mfa::Totp::configure(const Path &path,
                                 const Parameters &parameters) {
  return Vault::Sys::Mfa::configure(client_, getUrl(path), parameters);
}

std::optional<std::string>
Vault::Sys::Mfa::Totp::del(const Path &path, const Parameters &parameters) {
  return Vault::Sys::Mfa::del(client_, getUrl(path));
}

std::optional<std::string> Vault::Sys::Mfa::Totp::generate(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{path + "/generate"}));
}

std::optional<std::string>
Vault::Sys::Mfa::Totp::adminGenerate(const Path &path,
                                     const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{path + "/admin-generate"}),
                            parameters);
}

std::optional<std::string>
Vault::Sys::Mfa::Totp::adminDestroy(const Path &path,
                                    const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{path + "/admin-destroy"}),
                            parameters);
}

Vault::Url Vault::Sys::Mfa::Totp::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/mfa/method/totp/", path);
}
