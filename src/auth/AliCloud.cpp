#include "VaultClient.h"

std::optional<std::string> Vault::AliCloud::Auth::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::AliCloud::Auth::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string>
Vault::AliCloud::Auth::createRole(const Path &path,
                                  const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::AliCloud::Auth::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string>
Vault::AliCloud::Auth::login(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::AliCloud::Auth::getUrl(const Vault::Path &path) {
  return client_.getUrl("/v1/auth/alicloud/", path);
}
