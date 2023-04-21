#include "VaultClient.h"

std::optional<std::string> Vault::OCI::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::OCI::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string>
Vault::OCI::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::OCI::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::OCI::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"role"}));
}

std::optional<std::string> Vault::OCI::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::OCI::login(const Path &path,
                                             const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login/" + path}), parameters);
}

Vault::Url Vault::OCI::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/oci/", path);
}
