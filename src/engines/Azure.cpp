#include "VaultClient.h"

std::optional<std::string> Vault::Azure::Secrets::configure(const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Azure::Secrets::readConfiguration() {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Azure::Secrets::deleteConfiguration() {
  return Vault::HttpConsumer::del(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Azure::Secrets::createRole(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::Azure::Secrets::updateRole(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::Azure::Secrets::listRoles() {
  return Vault::HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::Azure::Secrets::generateCredentials(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"creds/" + path}));
}

Vault::Url Vault::Azure::Secrets::getUrl(const Path &path) {
  return client_.getUrl("/v1/azure/", path);
}
