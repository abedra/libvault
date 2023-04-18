#include "../../shared/shared.h"
#include <iostream>

int main(void) {
  char *rootTokenEnv = std::getenv("VAULT_ROOT_TOKEN");
  if (!rootTokenEnv) {
    std::cout << "The VAULT_ROOT_TOKEN environment variable must be set"
              << std::endl;
    exit(-1);
  }
  Vault::Token rootToken{rootTokenEnv};
  Vault::Client rootClient = getRootClient(rootToken);

  if (rootClient.is_authenticated()) {
    Vault::Sys sys{rootClient};
    auto response = sys.sealStatus();
    if (response) {
      std::cout << response.value() << std::endl;
    } else {
      std::cout << "Failure" << std::endl;
    }
  } else {
    std::cout << "Unable to authenticate" << std::endl;
  }
}
