#include <iostream>
#include "../../../lib/json.hpp"
#include "VaultClient.h"

Vault::Client getRootClient(const Vault::Token &rootToken) {
  Vault::TokenStrategy tokenStrategy{rootToken};
  Vault::Config config = Vault::ConfigBuilder()
      .withTlsEnabled(false)
      .build();
  return Vault::Client{config, tokenStrategy};
}

Vault::Client getAppRoleClient(const Vault::RoleId &roleId, const Vault::SecretId &secretId) {
  Vault::AppRoleStrategy authStrategy{roleId, secretId};
  Vault::Config config = Vault::ConfigBuilder()
      .withTlsEnabled(false)
      .build();
  return Vault::Client{config, authStrategy};
}

std::optional<std::string> enableAppRole(const Vault::Sys::Auth &authAdmin) {
  std::cout << "Enabling AppRole" << std::endl;
  return authAdmin.enable(Vault::Path{"approle"}, Vault::Parameters{{"type", "approle"}});
}

std::optional<std::string> disableAppRole(const Vault::Sys::Auth &authAdmin) {
  std::cout << "Disabling AppRole" << std::endl;
  return authAdmin.disable(Vault::Path{"approle"});
}

std::optional<std::string> createRole(const Vault::AppRole &appRoleAdmin) {
  std::cout << "Creating role 'example'" << std::endl;
  return appRoleAdmin.create(Vault::Path{"example"}, Vault::Parameters{});
}

std::optional<std::string> deleteRole(const Vault::AppRole &appRoleAdmin) {
  std::cout << "Deleting role 'example'" << std::endl;
  return appRoleAdmin.del(Vault::Path{"example"});
}

Vault::RoleId getRoleId(const Vault::AppRole &appRoleAdmin) {
  std::cout << "Fetching role id for 'example'" << std::endl;

  auto response = appRoleAdmin.getRoleId(Vault::Path{"example"});
  if (response) {
    return Vault::RoleId{nlohmann::json::parse(response.value())["data"]["role_id"]};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

Vault::SecretId getSecretId(const Vault::AppRole &appRoleAdmin) {
  std::cout << "Fetching secret id for 'example'" << std::endl;

  auto response = appRoleAdmin.generateSecretId(Vault::Path{"example"}, Vault::Parameters{});
  if (response) {
    return Vault::SecretId{nlohmann::json::parse(response.value())["data"]["secret_id"]};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

int main(void) {
  auto rootTokenEnv = std::getenv("VAULT_ROOT_TOKEN");

  if (!rootTokenEnv) {
    std::cout << "The VAULT_ROOT_TOKEN environment variable must be set" << std::endl;
    exit(-1);
  }

  auto rootToken = Vault::Token{rootTokenEnv};
  auto rootClient = getRootClient(rootToken);
  auto authAdmin = Vault::Sys::Auth{rootClient};
  auto appRoleAdmin = Vault::AppRole{rootClient};

  enableAppRole(authAdmin);
  createRole(appRoleAdmin);

  auto roleId = getRoleId(appRoleAdmin);
  auto secretId = getSecretId(appRoleAdmin);
  auto appRoleClient = getAppRoleClient(roleId, secretId);

  if (appRoleClient.is_authenticated()) {
    std::cout << "Authenticated: " << appRoleClient.getToken() << std::endl;
  } else {
    std::cout << "Unable to authenticate" << std::endl;
  }

  deleteRole(appRoleAdmin);
  disableAppRole(authAdmin);
}
