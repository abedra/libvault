#pragma once
#include <iostream>
#include "../../lib/json.hpp"
#include "VaultClient.h"

Vault::Client getRootClient(const Vault::Token &rootToken) {
  Vault::TokenStrategy tokenStrategy{rootToken};
  Vault::Config config = Vault::ConfigBuilder().withDebug(false).withTlsEnabled(false).build();

  return Vault::Client{config, tokenStrategy};
}

Vault::Client getAppRoleClient(const Vault::RoleId &roleId, const Vault::SecretId &secretId) {
  Vault::AppRoleStrategy authStrategy{roleId, secretId};
  Vault::Config config = Vault::ConfigBuilder().withTlsEnabled(false).build();

  return Vault::Client{config, authStrategy};
}

std::optional<std::string> createPolicy(const Vault::Sys::Policy &policyAdmin) {
  Vault::Parameters parameters{{
                                   "policy", "path \"secret/*\" {capabilities = [\"read\", \"update\", \"list\", \"delete\", \"create\"]}"
                               }};
  return policyAdmin.create(Vault::Path{"example"}, parameters);
}

std::optional<std::string> deletePolicy(const Vault::Sys::Policy &policyAdmin) {
  return policyAdmin.del(Vault::Path{"example"});
}

std::optional<std::string> enableAppRole(const Vault::Sys::Auth &authAdmin) {
  return authAdmin.enable(Vault::Path{"approle"}, Vault::Parameters{{"type", "approle"}});
}

std::optional<std::string> disableAppRole(const Vault::Sys::Auth &authAdmin) {
  return authAdmin.disable(Vault::Path{"approle"});
}

std::optional<std::string> createRole(const Vault::AppRole &appRoleAdmin) {
  Vault::Parameters parameters{{"token_policies", "example"}};
  return appRoleAdmin.create(Vault::Path{"example"}, parameters);
}

std::optional<std::string> deleteRole(const Vault::AppRole &appRoleAdmin) {
  return appRoleAdmin.del(Vault::Path{"example"});
}

Vault::RoleId getRoleId(const Vault::AppRole &appRoleAdmin) {
  auto response = appRoleAdmin.getRoleId(Vault::Path{"example"});
  if (response) {
    return Vault::RoleId{nlohmann::json::parse(response.value())["data"]["role_id"]};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

Vault::SecretId getSecretId(const Vault::AppRole &appRoleAdmin) {
  auto response = appRoleAdmin.generateSecretId(Vault::Path{"example"}, Vault::Parameters{});
  if (response) {
    return Vault::SecretId{nlohmann::json::parse(response.value())["data"]["secret_id"]};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

std::optional<std::string> enableKeyValue(const Vault::Sys::Mounts &mountAdmin) {
  return mountAdmin.enable(Vault::Path{}, Vault::Parameters{}, Vault::Parameters{}, Vault::Parameters{});
}

std::optional<std::string> disableKeyValue(const Vault::Sys::Mounts &mountAdmin) {
  return mountAdmin.disable(Vault::Path{});
}
