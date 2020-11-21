#include "VaultClient.h"

std::optional<std::string> Vault::Identity::Entity::create(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{}), parameters);
}

std::optional<std::string> Vault::Identity::Entity::readById(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"id/" + path}));
}

std::optional<std::string> Vault::Identity::Entity::updateById(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"id/" + path}), parameters);
}

std::optional<std::string> Vault::Identity::Entity::deleteById(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"id/" + path}));
}

std::optional<std::string> Vault::Identity::Entity::batchDeleteById(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"batch-delete"}), parameters);
}

std::optional<std::string> Vault::Identity::Entity::listById() {
  return HttpConsumer::list(client_, getUrl(Path{"id"}));
}

std::optional<std::string> Vault::Identity::Entity::createByName(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"name/" + path}), parameters);
}

std::optional<std::string> Vault::Identity::Entity::updateByName(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"name/" + path}), parameters);
}

std::optional<std::string> Vault::Identity::Entity::readByName(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"name/" + path}));
}

std::optional<std::string> Vault::Identity::Entity::deleteByName(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"name/" + path}));
}

std::optional<std::string> Vault::Identity::Entity::listByName(const Path &path) {
  return HttpConsumer::list(client_, getUrl(Path{"name"}));
}

std::optional<std::string> Vault::Identity::Entity::merge(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"merge"}), parameters);
}

Vault::Url Vault::Identity::Entity::getUrl(const Path &path) {
  return client_.getUrl("/v1/identity/entity/", path);
}
