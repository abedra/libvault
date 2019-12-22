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

void transit_encrypt_decrypt(VaultClient vaultClient) {
  Transit transit(vaultClient);
  Parameters parameters({ {"plaintext", Base64::encode("Attack at dawn")} });
  Vault::Path path("mykey");

  print_response(transit.encrypt(path, parameters));

  parameters = Parameters({ {"ciphertext", "vault:v1:wOWt0eYKlzLwVKitJchP9F456jMtiFZUc/tC8+0l5BE2SJLVw548yy6W"} });

  print_response(transit.decrypt(path, parameters));
}

void transit_keys(VaultClient vaultClient) {
  Transit transit(vaultClient);
  Vault::Path path("mykey");

  print_response(transit.generate_data_key(path, {{}}));
  print_response(transit.generate_wrapped_data_key(path, {{}}));
}

void transit_random(VaultClient vaultClient) {
  Transit transit(vaultClient);

  Parameters parameters({ {"format","base64"} });
  print_response(transit.generate_random_bytes(32, parameters));

  parameters = Parameters({ {"format","hex"} });
  print_response(transit.generate_random_bytes(32, parameters));
}

void transit_hash(VaultClient vaultClient) {
  Transit transit(vaultClient);
  auto input = Base64::encode("Attack at dawn");
  Parameters parameters({ {"format","base64"}, {"input", input} });

  print_response(transit.hash(Vault::Algorithms::SHA2_224, parameters));
  print_response(transit.hash(Vault::Algorithms::SHA2_256, parameters));
  print_response(transit.hash(Vault::Algorithms::SHA2_384, parameters));
  print_response(transit.hash(Vault::Algorithms::SHA2_512, parameters));

  parameters = Parameters({ {"format","hex"}, {"input", input} });

  print_response(transit.hash(Vault::Algorithms::SHA2_224, parameters));
  print_response(transit.hash(Vault::Algorithms::SHA2_256, parameters));
  print_response(transit.hash(Vault::Algorithms::SHA2_384, parameters));
  print_response(transit.hash(Vault::Algorithms::SHA2_512, parameters));
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

  transit_encrypt_decrypt(vaultClient);
  transit_keys(vaultClient);
  transit_random(vaultClient);
  transit_hash(vaultClient);
  transit_hmac(vaultClient);
}
