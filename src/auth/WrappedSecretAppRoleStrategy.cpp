#include <utility>
#include "VaultClient.h"

Vault::WrappedSecretAppRoleStrategy::WrappedSecretAppRoleStrategy(Vault::RoleId roleId, const Vault::Token &token)
  : roleId_(std::move(roleId))
  ,token_(token)
  {}

std::optional<Vault::AuthenticationResponse>
Vault::WrappedSecretAppRoleStrategy::authenticate(const Client &client) {
  Vault::Client unauthenticated(client, token_);
  Vault::Sys sys(client);

  auto secretId = sys.unwrap(unauthenticated);

  return secretId
         ? AppRoleStrategy(roleId_, secretId.value()).authenticate(client)
         : std::nullopt;
}
