#include <iostream>
#include "VaultClient.h"

template<typename T>
void print_response(std::optional<T> response) {
  if (response) {
    std::cout << response.value() << std::endl;
  } else {
    std::cout << "Error" << std::endl;
  }
}

void transit_hmac(VaultClient vaultClient) {
  Transit transit(vaultClient);
  Parameters parameters({ {"input", Base64::encode("Attack at dawn")} });
  Vault::Path key("mykey");

  print_response(transit.hmac(key, Vault::Algorithms::SHA2_224, parameters));
  print_response(transit.hmac(key, Vault::Algorithms::SHA2_256, parameters));
  print_response(transit.hmac(key, Vault::Algorithms::SHA2_384, parameters));
  print_response(transit.hmac(key, Vault::Algorithms::SHA2_512, parameters));
}

void transit_sign(VaultClient vaultClient) {
  Transit transit(vaultClient);
  Parameters parameters({ {"input", Base64::encode("Attack at dawn")} });
  Vault::Path key("mykey");

  print_response(transit.sign(key, Vault::Algorithms::SHA1, parameters));
  print_response(transit.sign(key, Vault::Algorithms::SHA2_224, parameters));
  print_response(transit.sign(key, Vault::Algorithms::SHA2_256, parameters));
  print_response(transit.sign(key, Vault::Algorithms::SHA2_384, parameters));
  print_response(transit.sign(key, Vault::Algorithms::SHA2_512, parameters));
}

static std::string getOrDefault(const char *name, std::string defaultValue) {
    auto value = std::getenv(name);

    return value ? value : defaultValue;
}

VaultClient loginWithAppRole(VaultConfig& config, HttpErrorCallback httpErrorCallback) {
  Vault::RoleId roleId{getOrDefault("APPROLE_ROLE_ID", "")};
  Vault::SecretId secretId{getOrDefault("APPROLE_SECRET_ID", "")};
  AppRoleStrategy authStrategy{roleId, secretId};

  return VaultClient{config, authStrategy, httpErrorCallback};
}

VaultClient loginWithWrappedAppRole(VaultConfig& config, HttpErrorCallback httpErrorCallback) {
  Vault::RoleId roleId{getOrDefault("APPROLE_ROLE_ID", "")};
  Vault::Token wrappedToken{getOrDefault("APPROLE_WRAPPED_TOKEN", "")};
  WrappedSecretAppRoleStrategy wrappedAuthStrategy{roleId, wrappedToken};

  return VaultClient{config, wrappedAuthStrategy, httpErrorCallback};
}

VaultClient loginWithLdap(VaultConfig& config, HttpErrorCallback httpErrorCallback) {
  auto username = getOrDefault("LDAP_USERNAME", "");
  auto password = getOrDefault("LDAP_PASSWORD", "");
  auto ldapStrategy = LdapStrategy{username, password};

  return VaultClient{config, ldapStrategy, httpErrorCallback};
}

int main() {
  HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };

  auto config = VaultConfigBuilder()
    .withHost(Vault::Host{"localhost"})
    .withTlsEnabled(false)
    .build();

  auto vaultClient = loginWithAppRole(config, httpErrorCallback);
  // auto vaultClient = loginWithWrappedAppRole(config, httpErrorCallback)
  // auto vaultClient = loginWithLdap(config, httpErrorCallback)

  transit_hmac(vaultClient);
  transit_sign(vaultClient);
}
