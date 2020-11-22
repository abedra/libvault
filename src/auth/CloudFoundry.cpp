#include "VaultClient.h"

std::optional<std::string> Vault::CloudFoundry::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::CloudFoundry::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::CloudFoundry::deleteConfig() {
  return HttpConsumer::del(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::CloudFoundry::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::CloudFoundry::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::CloudFoundry::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::CloudFoundry::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::CloudFoundry::login(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::CloudFoundry::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/cf/", path);
}
