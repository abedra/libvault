#include "VaultClient.h"

std::optional<std::string> Vault::Azure::configure(const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Azure::readConfiguration() {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Azure::deleteConfiguration() {
  return Vault::HttpConsumer::del(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Azure::createRole(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::Azure::updateRole(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::Azure::listRoles() {
  return Vault::HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::Azure::generateCredentials(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"creds/" + path}));
}

Vault::Url Vault::Azure::getUrl(const Path &path) {
  return client_.getUrl("/v1/azure/", path);
}
