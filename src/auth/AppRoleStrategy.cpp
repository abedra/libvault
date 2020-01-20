#include <utility>
#include "json.hpp"
#include "VaultClient.h"

Vault::AppRoleStrategy::AppRoleStrategy(Vault::RoleId roleId, Vault::SecretId secretId)
  : roleId_(std::move(roleId))
  , secretId_(std::move(secretId))
  {}

std::optional<Vault::AuthenticationResponse>
Vault::AppRoleStrategy::authenticate(const Vault::Client& client) {
  return Vault::HttpConsumer::authenticate(
    client,
    getUrl(client, Vault::Path{"/login"}),
    [&]() {
      nlohmann::json j;
      j = nlohmann::json::object();
      j["role_id"] = roleId_.value();
      j["secret_id"] = secretId_.value();
      return j.dump();
    }
  );
}

Vault::Url Vault::AppRoleStrategy::getUrl(const Vault::Client& client, const Vault::Path& path) {
  return client.getUrl("/v1/auth/approle", path);
}
