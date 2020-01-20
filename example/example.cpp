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
  Vault::KeyValue kv{vaultClient, Vault::KeyValue::Version::v1};
  Vault::Path mount{"/test"};
  Vault::KeyValue kv2{vaultClient, mount};
  Vault::Parameters parameters(
    {
      {"foo","world"},
      {"baz","quux"},
      {"something", "something else"},
    }
  );

  Vault::Path key{"hello"};

  kv.create(key, parameters);
  kv2.create(key, parameters);

  std::cout << kv.read(key).value() << std::endl;
  std::cout << kv2.read(key).value() << std::endl;
}
