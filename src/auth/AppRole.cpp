#include "json.hpp"
#include "VaultClient.h"

Vault::AppRole::AppRole(const Client& vaultClient)
  : client_(vaultClient)
  {}

std::optional<std::string> Vault::AppRole::list() {
  return Vault::HttpConsumer::list(client_, getUrl(Vault::Path{}));
}

std::optional<std::string>
Vault::AppRole::create(const Vault::Path& roleName, const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(roleName),
    parameters,
    [&](const Parameters& params) {
      nlohmann::json json;
      json["data"] = nlohmann::json::object();
      for (auto&& [key, value] : params) {
        json["data"][key] = value;
      }
      return json.dump();
    }
  );
}

std::optional<std::string>
Vault::AppRole::update(const Vault::Path& roleName, const Parameters& parameters) {
  return create(roleName, parameters);
}

std::optional<std::string>
Vault::AppRole::read(const Vault::Path& roleName) {
  return Vault::HttpConsumer::get(client_, getUrl(roleName));
}

std::optional<std::string>
Vault::AppRole::del(const Vault::Path &roleName) {
  return Vault::HttpConsumer::del(client_, getUrl(roleName));
}

std::optional<std::string>
Vault::AppRole::getRoleId(const Vault::Path &roleName) {
  return Vault::HttpConsumer::get(client_, getUrl(Vault::Path{roleName + "/role-id"}));
}

std::optional<std::string>
Vault::AppRole::updateRoleId(const Vault::Path &roleName, const Parameters& parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Vault::Path{roleName + "/role-id"}), parameters);
}

std::optional<std::string>
Vault::AppRole::getRoleProperty(const Vault::Path &roleName, const Vault::Path &propertyName) {
  return Vault::HttpConsumer::get(client_, getUrl(Vault::Path{roleName + propertyName}));
}

std::optional<std::string>
Vault::AppRole::updateRoleProperty(const Vault::Path &roleName,
                                   const Vault::Path &propertyName,
                                   const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Vault::Path{roleName + propertyName}), parameters);
}

std::optional<std::string>
Vault::AppRole::deleteRoleProperty(const Vault::Path &roleName, const Vault::Path &propertyName) {
  return Vault::HttpConsumer::del(client_, getUrl(Vault::Path{roleName + propertyName}));
}

std::optional<std::string>
Vault::AppRole::generateSecretId(const Vault::Path &roleName, const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id"}),
    parameters
  );
}

std::optional<std::string>
Vault::AppRole::listSecretAccessors(const Vault::Path &roleName) {
  return Vault::HttpConsumer::list(client_, getUrl(Vault::Path{roleName + "/secret-id"}));
}

std::optional<std::string>
Vault::AppRole::readSecretId(const Vault::Path &roleName, const Parameters &parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id/lookup"}),
    parameters
  );
}

std::optional<std::string>
Vault::AppRole::destroySecretId(const Vault::Path &roleName, const Parameters &parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id/destroy"}),
    parameters
  );
}

std::optional<std::string>
Vault::AppRole::readSecretIdAccessor(const Vault::Path &roleName, const Parameters &parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id-accessor/lookup"}),
    parameters
  );
}

std::optional<std::string>
Vault::AppRole::destroySecretIdAccessor(const Vault::Path &roleName, const Parameters &parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id-accessor/destroy"}),
    parameters
  );
}

std::optional<std::string>
Vault::AppRole::customSecretId(const Vault::Path &roleName, const Parameters &parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/custom-secret-id"}),
    parameters
  );
}

std::optional<std::string>
Vault::AppRole::tidy(const Vault::Path &roleName) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{roleName + "/secret-id"}),
    Parameters{}
  );
}

Vault::Url Vault::AppRole::getUrl(const Vault::Path& path) {
  return client_.getUrl("/v1/auth/approle/role/", path);
}
