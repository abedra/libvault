#include <utility>
#include "VaultClient.h"

WrappedSecretAppRoleStrategy::WrappedSecretAppRoleStrategy(Vault::RoleId roleId, const Vault::Token& token)
  : roleId_(std::move(roleId))
  , token_(token)
  {}

std::optional<AuthenticationResponse> WrappedSecretAppRoleStrategy::authenticate(const VaultClient& vaultClient) {
  VaultClient temp(vaultClient, token_);

  auto secret_id = Unwrap::unwrap(temp);

  return secret_id
    ? AppRoleStrategy(roleId_, secret_id.value()).authenticate(vaultClient)
    : std::nullopt;
}
