#include <utility>
#include "json.hpp"
#include "VaultClient.h"

AppRoleStrategy::AppRoleStrategy(Vault::RoleId roleId, Vault::SecretId secretId)
  : roleId_(std::move(roleId))
  , secretId_(std::move(secretId))
  {}

std::optional<AuthenticationResponse> AppRoleStrategy::authenticate(const VaultClient& client) {
  return VaultHttpConsumer::authenticate(
    client,
    getUrl(client, Vault::Path{"/login"}),
    [&]() {
      nlohmann::json j;
      j = nlohmann::json::object();
      j["role_id"] = roleId_.value();
      j["secret_id"] = secretId_.value();
      return j;
    }
  );
}

Vault::Url AppRoleStrategy::getUrl(const VaultClient& client, const Vault::Path& path) {
  return client.getUrl("/v1/auth/approle", path);
}
