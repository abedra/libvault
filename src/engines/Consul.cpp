#include "VaultClient.h"

std::optional<std::string> Vault::Consul::configure(const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"config/access"}), parameters);
}

std::optional<std::string> Vault::Consul::createRole(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::Consul::updateRole(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::Consul::readRole(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::Consul::listRoles() {
  return Vault::HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::Consul::deleteRole(const Path &path) {
  return Vault::HttpConsumer::del(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::Consul::generateCredential(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"creds/" + path}));
}

Vault::Url Vault::Consul::getUrl(const Path &path) {
  return client_.getUrl("/v1/consul/", path);
}