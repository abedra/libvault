#include "VaultClient.h"

std::optional<std::string> Vault::AliCloud::Secrets::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string>
Vault::AliCloud::Secrets::createConfig(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::AliCloud::Secrets::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string>
Vault::AliCloud::Secrets::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string>
Vault::AliCloud::Secrets::createRole(const Path &path,
                                     const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string>
Vault::AliCloud::Secrets::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string>
Vault::AliCloud::Secrets::generateCredentials(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"creds/" + path}));
}

Vault::Url Vault::AliCloud::Secrets::getUrl(const Vault::Path &path) {
  return client_.getUrl("/v1/alicloud", path);
}
