#include "VaultClient.h"

std::optional<std::string> Vault::MongoDBAtlas::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::MongoDBAtlas::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::MongoDBAtlas::updateRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::MongoDBAtlas::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::MongoDBAtlas::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::MongoDBAtlas::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::MongoDBAtlas::readCredential(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"creds/" + path}));
}

Vault::Url Vault::MongoDBAtlas::getUrl(const Path &path) {
  return client_.getUrl("/v1/mongodbatlas/", path);
}
