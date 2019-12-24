#include <iostream>
#include "VaultClient.h"

int main(void)
{
  HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };

  auto config = VaultConfigBuilder().build();
  AppRoleStrategy authStrategy{Vault::RoleId{"<role_id>"}, Vault::SecretId{"<secret_id>"}};
  VaultClient vaultClient{config, authStrategy, httpErrorCallback};

  KeyValue kv{vaultClient, KeyValue::Version::v1};
  Vault::Path mount{"/test"};
  KeyValue kv2{vaultClient, mount};

  Parameters parameters(
    {
      {"foo","world"},
      {"baz","quux"},
      {"something", "something else"},
    });

  Vault::Path key{"hello"};

  kv.put(key, parameters);
  kv2.put(key, parameters);

  std::cout << kv.get(key).value() << std::endl;
  std::cout << kv2.get(key).value() << std::endl;
}
