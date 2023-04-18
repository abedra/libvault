#include "../../shared/shared.h"
#include <iostream>

Vault::Client setup(const Vault::Client &rootClient,
                    const Vault::Path &appRoleMount,
                    const Vault::SecretMount &secretMount) {
  Vault::Sys::Auth authAdmin{rootClient};
  Vault::AppRole appRoleAdmin{rootClient};
  Vault::Sys::Policy policyAdmin{rootClient};
  Vault::Sys::Mounts mountAdmin{rootClient};

  createPolicy(policyAdmin);
  enableAppRole(authAdmin, appRoleMount);
  createRole(appRoleAdmin);
  enableKeyValue(mountAdmin, secretMount);

  Vault::RoleId roleId = getRoleId(appRoleAdmin);
  Vault::SecretId secretId = getSecretId(appRoleAdmin);

  return getAppRoleClient(roleId, secretId, appRoleMount);
}

void cleanup(const Vault::Client &rootClient, const Vault::Path &appRoleMount,
             const Vault::SecretMount &secretMount) {
  Vault::Sys::Auth authAdmin = Vault::Sys::Auth{rootClient};
  Vault::AppRole appRoleAdmin = Vault::AppRole{rootClient};
  Vault::Sys::Mounts mountAdmin{rootClient};
  Vault::Sys::Policy policyAdmin{rootClient};

  deleteRole(appRoleAdmin);
  disableAppRole(authAdmin, appRoleMount);
  disableKeyValue(mountAdmin, secretMount);
  deletePolicy(policyAdmin);
}

int main(void) {
  char *rootTokenEnv = std::getenv("VAULT_ROOT_TOKEN");
  if (!rootTokenEnv) {
    std::cout << "The VAULT_ROOT_TOKEN environment variable must be set"
              << std::endl;
    exit(-1);
  }
  Vault::Token rootToken{rootTokenEnv};
  Vault::Client rootClient = getRootClient(rootToken);
  Vault::Path appRoleMount{"approle"};
  Vault::SecretMount secretMount{"kv"};
  Vault::Client client = setup(rootClient, appRoleMount, secretMount);
  Vault::KeyValue kv{client, secretMount};
  Vault::Path key{"hello"};
  Vault::Parameters parameters(
      {{"foo", "world"}, {"baz", "quux"}, {"something", "something else"}});

  kv.create(key, parameters);
  auto response = kv.read(key);
  if (response) {
    std::cout << response.value() << std::endl;
  } else {
    std::cout << "Unable to read secrets" << std::endl;
  }

  cleanup(rootClient, appRoleMount, secretMount);
}
