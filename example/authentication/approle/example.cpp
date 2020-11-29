#include <iostream>
#include "../../../lib/json.hpp"
#include "VaultClient.h"

Vault::Client getRootClient() {
  auto rootTokenEnv = std::getenv("VAULT_SINGLE_TOKEN");

  if (!rootTokenEnv) {
    std::cout << "The VAULT_ROOT_TOKEN environment variable must be set" << std::endl;
  }

  Vault::TokenStrategy tokenStrategy{Vault::Token{rootTokenEnv}};

  Vault::Config config = Vault::ConfigBuilder()
      .withDebug(false)
      .withTlsEnabled(false)
      .build();

  Vault::HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };

  return Vault::Client{config, tokenStrategy, httpErrorCallback};
}

Vault::Client getAppRoleClient(const Vault::RoleId &roleId, const Vault::SecretId &secretId) {
  Vault::AppRoleStrategy authStrategy{roleId, secretId};

  Vault::Config config = Vault::ConfigBuilder()
      .withDebug(false)
      .withTlsEnabled(false)
      .build();

  Vault::HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };

  return Vault::Client{config, authStrategy, httpErrorCallback};
}

void enableAppRole(const Vault::Client &client) {
  std::cout << "Enabling AppRole" << std::endl;

  Vault::Sys::Auth authAdmin{client};
  authAdmin.enable(Vault::Path{"approle"}, Vault::Parameters{{"type", "approle"}});
}

void disableAppRole(const Vault::Client &client) {
  std::cout << "Disabling AppRole" << std::endl;

  Vault::Sys::Auth authAdmin{client};
  authAdmin.disable(Vault::Path{"approle"});
}

void createRole(const Vault::Client &client) {
  std::cout << "Creating role 'example'" << std::endl;

  auto appRoleAdmin = Vault::AppRole{client};
  appRoleAdmin.create(Vault::Path{"example"}, Vault::Parameters{});
}

Vault::RoleId getRoleId(const Vault::Client &client) {
  std::cout << "Fetching role id for 'example'" << std::endl;

  auto appRoleAdmin = Vault::AppRole{client};
  auto response = appRoleAdmin.getRoleId(Vault::Path{"example"});

  if (response) {
    std::string roleId = nlohmann::json::parse(response.value())["data"]["role_id"];
    return Vault::RoleId{roleId};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

Vault::SecretId getSecretId(const Vault::Client &client) {
  std::cout << "Fetching secret id for 'example'" << std::endl;

  auto appRoleAdmin = Vault::AppRole{client};
  auto response = appRoleAdmin.generateSecretId(Vault::Path{"example"}, Vault::Parameters{});

  if (response) {
    std::string secretId = nlohmann::json::parse(response.value())["data"]["secret_id"];
    return Vault::SecretId{secretId};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

int main(void) {
  auto rootClient = getRootClient();
  enableAppRole(rootClient);
  createRole(rootClient);
  auto roleId = getRoleId(rootClient);
  auto secretId = getSecretId(rootClient);
  auto appRoleClient = getAppRoleClient(roleId, secretId);

  if (appRoleClient.is_authenticated()) {
    std::cout << "Authenticated: " << appRoleClient.getToken() << std::endl;
  } else {
    std::cout << "Unable to authenticate" << std::endl;
  }

  disableAppRole(rootClient);
}
