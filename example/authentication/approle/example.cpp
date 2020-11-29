#include <iostream>
#include "VaultClient.h"
#include "../../shared/shared.h"

Vault::Client setup(const Vault::Client &rootClient) {
  Vault::Sys::Auth authAdmin{rootClient};
  Vault::AppRole appRoleAdmin{rootClient};

  enableAppRole(authAdmin);
  createRole(appRoleAdmin);

  Vault::RoleId roleId = getRoleId(appRoleAdmin);
  Vault::SecretId secretId = getSecretId(appRoleAdmin);

  return getAppRoleClient(roleId, secretId);
}

void cleanup(const Vault::Client &rootClient) {
  Vault::Sys::Auth authAdmin = Vault::Sys::Auth{rootClient};
  Vault::AppRole appRoleAdmin = Vault::AppRole{rootClient};

  deleteRole(appRoleAdmin);
  disableAppRole(authAdmin);
}

int main(void) {
  char *rootTokenEnv = std::getenv("VAULT_ROOT_TOKEN");
  if (!rootTokenEnv) {
    std::cout << "The VAULT_ROOT_TOKEN environment variable must be set" << std::endl;
    exit(-1);
  }
  Vault::Token rootToken{rootTokenEnv};
  Vault::Client rootClient = getRootClient(rootToken);
  Vault::Client appRoleClient = setup(rootClient);

  if (appRoleClient.is_authenticated()) {
    std::cout << "Authenticated: " << appRoleClient.getToken() << std::endl;
  } else {
    std::cout << "Unable to authenticate" << std::endl;
  }

  cleanup(rootClient);
}
