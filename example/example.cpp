#include <iostream>
#include "VaultClient.h"

int main(void)
{
  Vault::HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };

  Vault::Config config = Vault::ConfigBuilder().build();
  Vault::AppRoleStrategy authStrategy{
    Vault::RoleId{"<role_id>"},
    Vault::SecretId{"<secret_id>"}
  };
  Vault::Client vaultClient{config, authStrategy, httpErrorCallback};

  Vault::KeyValue kv{vaultClient, Vault::KeyValue::Version::v1};
  Vault::Path mount{"/test"};
  Vault::KeyValue kv2{vaultClient, mount};

  Vault::Parameters parameters(
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
