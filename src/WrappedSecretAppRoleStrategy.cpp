#include <utility>
#include "VaultClient.h"

WrappedSecretAppRoleStrategy::WrappedSecretAppRoleStrategy(RoleId roleId, const Token& token)
  : roleId_(std::move(roleId))
  , token_(token)
  {}

optional<AuthenticationResponse> WrappedSecretAppRoleStrategy::authenticate(const VaultClient& vaultClient) {
  VaultClient temp(vaultClient, token_);

  auto secret_id = Unwrap::unwrap(temp);

  return secret_id
    ? AppRoleStrategy(roleId_, secret_id.value()).authenticate(vaultClient)
    : std::nullopt;
}
