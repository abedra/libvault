#include <utility>
#include "VaultClient.h"

Vault::WrappedSecretAppRoleStrategy::WrappedSecretAppRoleStrategy(Vault::RoleId roleId, const Vault::Token& token)
  : roleId_(std::move(roleId))
  , token_(token)
  {}

std::optional<Vault::AuthenticationResponse>
Vault::WrappedSecretAppRoleStrategy::authenticate(const Client& client) {
  Vault::Client unauthenticated(client, token_);

  auto secret_id = Unwrap::unwrap(unauthenticated);

  return secret_id
    ? AppRoleStrategy(roleId_, secret_id.value()).authenticate(client)
    : std::nullopt;
}
