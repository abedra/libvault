#include "VaultClient.h"

std::optional<std::string> Vault::Identity::GroupAlias::create(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{}), parameters);
}

std::optional<std::string> Vault::Identity::GroupAlias::readById(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"id/" + path}));
}

std::optional<std::string> Vault::Identity::GroupAlias::updateById(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"id/" + path}), parameters);
}

std::optional<std::string> Vault::Identity::GroupAlias::deleteById(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"id/" + path}));
}

std::optional<std::string> Vault::Identity::GroupAlias::listById() {
  return HttpConsumer::list(client_, getUrl(Path{"id"}));
}

Vault::Url Vault::Identity::GroupAlias::getUrl(const Path &path) {
  return client_.getUrl("/v1/identity/group-alias/", path);
}