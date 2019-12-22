#pragma once

#include <iostream>
#include "VaultClient.h"

static std::string getOrDefault(const char *name, const std::string& defaultValue) {
  auto value = std::getenv(name);

  return value ? value : defaultValue;
}

template<typename T>
void print_response(std::optional<T> response) {
  if (response) {
    std::cout << response.value() << std::endl;
  } else {
    std::cout << "Error" << std::endl;
  }
}

inline VaultClient login() {
  Vault::RoleId roleId{getOrDefault("APPROLE_ROLE_ID", "")};
  Vault::SecretId secretId{getOrDefault("APPROLE_SECRET_ID", "")};
  AppRoleStrategy authStrategy{roleId, secretId};
  HttpErrorCallback httpErrorCallback = [&](const std::string& err) { std::cout << err << std::endl; };

  VaultConfig config = VaultConfigBuilder()
      .withHost(Vault::Host{"localhost"})
      .withTlsEnabled(false)
      .withDebug(true)
      .build();

  return VaultClient{config, authStrategy, httpErrorCallback};
}