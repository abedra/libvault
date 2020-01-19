#include <utility>
#include "json.hpp"
#include "VaultClient.h"

AppRole::AppRole(const VaultClient& vaultClient)
  : client_(vaultClient)
  {}

std::optional<std::string> AppRole::list() {
  return VaultHttpConsumer::list(client_, getUrl(Vault::Path{"/"}));
}

std::optional<std::string> AppRole::create(const Vault::Path& roleName, const Parameters& parameters) {
  return VaultHttpConsumer::post(client_, getUrl(roleName), parameters, [&](auto params) {
    nlohmann::json j;
    j["data"] = nlohmann::json::object();
    std::for_each(
      parameters.begin(),
      parameters.end(),
      [&](std::pair<std::string, std::string> pair) {
        j["data"][pair.first] = pair.second;
      }
    );
    return j;
  });
}

std::optional<std::string> AppRole::update(const Vault::Path& roleName, const Parameters& parameters) {
  return create(roleName, parameters);
}

std::optional<std::string> AppRole::read(const Vault::Path& roleName) {
  return VaultHttpConsumer::get(client_, getUrl(roleName));
}

std::optional<std::string> AppRole::del(const Vault::Path &roleName) {
  return VaultHttpConsumer::del(client_, getUrl(roleName));
}

std::optional<std::string> AppRole::getRoleId(const Vault::Path &roleName) {
  return VaultHttpConsumer::get(client_, getUrl(Vault::Path{roleName + "/role-id"}));
}

std::optional<std::string> AppRole::updateRoleId(const Vault::Path &roleName, const Parameters& parameters) {
  return VaultHttpConsumer::post(client_, getUrl(Vault::Path{roleName + "/role-id"}), parameters);
}

std::optional<std::string> AppRole::getRoleProperty(const Vault::Path &roleName, const Vault::Path &propertyName) {
  return VaultHttpConsumer::get(client_, getUrl(Vault::Path{roleName + propertyName}));
}

std::optional<std::string> AppRole::updateRoleProperty(const Vault::Path &roleName,
                                                       const Vault::Path &propertyName,
                                                       const Parameters &parameters) {
  return VaultHttpConsumer::post(client_, getUrl(Vault::Path{roleName + propertyName}), parameters);
}

std::optional<std::string> AppRole::deleteRoleProperty(const Vault::Path &roleName, const Vault::Path &propertyName) {
  return VaultHttpConsumer::del(client_, getUrl(Vault::Path{roleName + propertyName}));
}

std::optional<std::string> AppRole::generateSecretId(const Vault::Path &roleName, const Parameters& parameters) {
  return VaultHttpConsumer::post(client_, getUrl(Vault::Path{roleName + "/secret-id"}), parameters);
}

std::optional<std::string> AppRole::listSecretAccessors(const Vault::Path &roleName) {
  return VaultHttpConsumer::list(client_, getUrl(Vault::Path{roleName + "/secret-id"}));
}

std::optional<std::string> AppRole::readSecretId(const Vault::Path &roleName, const Parameters &parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id/lookup"}),
    parameters
  );
}

std::optional<std::string> AppRole::destroySecretId(const Vault::Path &roleName, const Parameters &parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id/destroy"}),
    parameters
  );
}

std::optional<std::string> AppRole::readSecretIdAccessor(const Vault::Path &roleName, const Parameters &parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id-accessor/lookup"}),
    parameters
  );
}

std::optional<std::string> AppRole::destroySecretIdAccessor(const Vault::Path &roleName, const Parameters &parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id-accessor/destroy"}),
    parameters
  );
}

std::optional<std::string> AppRole::customSecretId(const Vault::Path &roleName, const Parameters &parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/custom-secret-id"}),
    parameters
  );
}

std::optional<std::string> AppRole::tidy(const Vault::Path &roleName) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id"}),
    Parameters{}
  );
}

Vault::Url AppRole::getUrl(const Vault::Path& path) {
  return client_.getUrl("/v1/auth/approle/role", path);
}
