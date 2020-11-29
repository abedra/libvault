#include <iostream>
#include "VaultClient.h"

int main(void)
{
  Vault::HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };

  Vault::AppRoleStrategy authStrategy{
    Vault::RoleId{"<role_id>"},
    Vault::SecretId{"<secret_id>"}
  };

  Vault::Config config = Vault::ConfigBuilder().build();
  Vault::Client vaultClient{config, authStrategy, httpErrorCallback};
  Vault::SecretMount mount{"/test"};
  Vault::KeyValue kv{vaultClient, mount};
  Vault::Path key{"hello"};
  Vault::Parameters parameters(
    {
      {"foo","world"},
      {"baz","quux"},
      {"something", "something else"},
    }
  );

  kv.create(key, parameters);

  std::cout << kv.read(key).value() << std::endl;
}
