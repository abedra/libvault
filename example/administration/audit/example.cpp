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
    Vault::Sys::Audit auditAdmin{rootClient};
    auditAdmin.enable(Vault::Path{"file"}, Vault::Parameters{{"type", "file"}}, Vault::Parameters{{"file_path", "/vault/logs/audit.log"}});

    auto response = auditAdmin.read();
    if (response) {
      std::cout << response.value() << std::endl;
      auditAdmin.disable(Vault::Path{"file"});
    } else {
      std::cout << "Failure" << std::endl;
    }
  } else {
    std::cout << "Unable to authenticate" << std::endl;
  }
}
