#include <utility>

#include "json.hpp"
#include "VaultClient.h"

Vault::WrappedSecretAppRoleStrategy::WrappedSecretAppRoleStrategy(Vault::RoleId roleId, const Vault::Token &token)
  : roleId_(std::move(roleId))
  ,token_(token)
  {}

std::optional<Vault::AuthenticationResponse>
Vault::WrappedSecretAppRoleStrategy::authenticate(const Client &client) {
  Vault::Sys sys(client);

  auto unwrapped = sys.unwrap(token_);

  if (unwrapped) {
    auto secretId = Vault::SecretId{nlohmann::json::parse(unwrapped.value())["data"]["secret_id"]};
    return AppRoleStrategy(roleId_, secretId).authenticate(client);
  } else {
    return std::nullopt;
  }
}
