#include "VaultClient.h"

std::optional<std::string>
Vault::Identity::Group::create(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{}), parameters);
}

std::optional<std::string> Vault::Identity::Group::readById(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"id/" + path}));
}

std::optional<std::string>
Vault::Identity::Group::updateById(const Path &path,
                                   const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"id/" + path}), parameters);
}

std::optional<std::string>
Vault::Identity::Group::deleteById(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"id/" + path}));
}

std::optional<std::string> Vault::Identity::Group::listById() {
  return HttpConsumer::list(client_, getUrl(Path{"id"}));
}

std::optional<std::string>
Vault::Identity::Group::createByName(const Path &path,
                                     const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"name/" + path}), parameters);
}

std::optional<std::string>
Vault::Identity::Group::updateByName(const Path &path,
                                     const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"name/" + path}), parameters);
}

std::optional<std::string>
Vault::Identity::Group::readByName(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"name/" + path}));
}

std::optional<std::string>
Vault::Identity::Group::deleteByName(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"name/" + path}));
}

std::optional<std::string>
Vault::Identity::Group::listByName(const Path &path) {
  return HttpConsumer::list(client_, getUrl(Path{"name"}));
}

Vault::Url Vault::Identity::Group::getUrl(const Path &path) {
  return client_.getUrl("/v1/identity/group/", path);
}
