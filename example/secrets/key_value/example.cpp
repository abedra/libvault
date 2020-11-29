#include <iostream>
#include "VaultClient.h"
#include "../../shared/shared.h"

Vault::Client setup(const Vault::Client &rootClient) {
  Vault::Sys::Auth authAdmin{rootClient};
  Vault::AppRole appRoleAdmin{rootClient};
  Vault::Sys::Policy policyAdmin{rootClient};
  Vault::Sys::Mounts mountAdmin{rootClient};

  createPolicy(policyAdmin);
  enableAppRole(authAdmin);
  createRole(appRoleAdmin);
  enableKeyValue(mountAdmin);

  Vault::RoleId roleId = getRoleId(appRoleAdmin);
  Vault::SecretId secretId = getSecretId(appRoleAdmin);

  return getAppRoleClient(roleId, secretId);
}

void cleanup(const Vault::Client &rootClient) {
  Vault::Sys::Auth authAdmin = Vault::Sys::Auth{rootClient};
  Vault::AppRole appRoleAdmin = Vault::AppRole{rootClient};
  Vault::Sys::Mounts mountAdmin{rootClient};
  Vault::Sys::Policy policyAdmin{rootClient};

  deleteRole(appRoleAdmin);
  disableAppRole(authAdmin);
  disableKeyValue(mountAdmin);
  deletePolicy(policyAdmin);
}

int main(void) {
  char *rootTokenEnv = std::getenv("VAULT_ROOT_TOKEN");
  if (!rootTokenEnv) {
    std::cout << "The VAULT_ROOT_TOKEN environment variable must be set" << std::endl;
    exit(-1);
  }
  Vault::Token rootToken{rootTokenEnv};
  Vault::Client rootClient = getRootClient(rootToken);
  Vault::Client client = setup(rootClient);
  Vault::KeyValue kv{client};
  Vault::Path key{"hello"};
  Vault::Parameters parameters(
      {{"foo",       "world"},
       {"baz",       "quux"},
       {"something", "something else"}});

  kv.create(key, parameters);
  auto response = kv.read(key);
  if (response) {
    std::cout << response.value() << std::endl;
  } else {
    std::cout << "Unable to read secrets" << std::endl;
  }

  cleanup(rootClient);
}
