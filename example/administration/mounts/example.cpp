#include <iostream>
#include "VaultClient.h"
#include "../../shared/shared.h"

int main(void) {
  char *rootTokenEnv = std::getenv("VAULT_ROOT_TOKEN");
  if (!rootTokenEnv) {
    std::cout << "The VAULT_ROOT_TOKEN environment variable must be set" << std::endl;
    exit(-1);
  }
  Vault::Token rootToken{rootTokenEnv};
  Vault::Client rootClient = getRootClient(rootToken);

  if (rootClient.is_authenticated()) {
    Vault::Sys::Mounts mountAdmin{rootClient};
    mountAdmin.enable(
        Vault::Path{"example"},
        Vault::Parameters{{"type", "kv"}},
        Vault::Parameters{{"version", "2"}},
        Vault::Parameters{{"max_lease_ttl", "5m"}});

    auto response = mountAdmin.list();
    if (response) {
      std::cout << response.value() << std::endl;
      mountAdmin.disable(Vault::Path{"example"});
    } else {
      std::cout << "Failure" << std::endl;
    }
  } else {
    std::cout << "Unable to authenticate" << std::endl;
  }
}
