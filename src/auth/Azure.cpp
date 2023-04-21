#include "VaultClient.h"

std::optional<std::string>
Vault::Azure::Auth::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Azure::Auth::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Azure::Auth::deleteConfig() {
  return HttpConsumer::del(client_, getUrl(Path{"config"}));
}

std::optional<std::string>
Vault::Azure::Auth::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::Azure::Auth::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::Azure::Auth::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"role"}));
}

std::optional<std::string> Vault::Azure::Auth::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string>
Vault::Azure::Auth::login(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::Azure::Auth::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/azure/", path);
}
