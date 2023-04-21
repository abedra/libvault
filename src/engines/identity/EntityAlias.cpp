#include "VaultClient.h"

std::optional<std::string>
Vault::Identity::EntityAlias::create(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{}), parameters);
}

std::optional<std::string>
Vault::Identity::EntityAlias::readById(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"id/" + path}));
}

std::optional<std::string>
Vault::Identity::EntityAlias::updateById(const Path &path,
                                         const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"id/" + path}), parameters);
}

std::optional<std::string>
Vault::Identity::EntityAlias::deleteById(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"id/" + path}));
}

std::optional<std::string> Vault::Identity::EntityAlias::listById() {
  return HttpConsumer::list(client_, getUrl(Path{"id"}));
}

Vault::Url Vault::Identity::EntityAlias::getUrl(const Path &path) {
  return client_.getUrl("/v1/identity/entity-alias/", path);
}
