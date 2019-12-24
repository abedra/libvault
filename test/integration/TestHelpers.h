#pragma once

#include <iostream>
#include <catch2/catch.hpp>

#include "json.hpp"
#include "VaultClient.h"

static std::string getOrDefault(const char *name, const std::string &defaultValue) {
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

namespace TestHelpers {
  namespace AppRole {
    inline VaultClient login() {
      Vault::RoleId roleId{getOrDefault("APPROLE_ROLE_ID", "")};
      Vault::SecretId secretId{getOrDefault("APPROLE_SECRET_ID", "")};
      AppRoleStrategy authStrategy{roleId, secretId};
      HttpErrorCallback httpErrorCallback = [&](const std::string &err) { std::cout << err << std::endl; };

      VaultConfig config = VaultConfigBuilder()
        .withHost(Vault::Host{"localhost"})
        .withTlsEnabled(false)
          //.withDebug(true)
        .build();

      return VaultClient{config, authStrategy, httpErrorCallback};
    }
  }

  namespace Token {
    inline VaultClient login() {
      Vault::Token token{getOrDefault("VAULT_SINGLE_TOKEN", "")};
      TokenStrategy tokenStrategy{token};
      HttpErrorCallback httpErrorCallback = [&](const std::string &err) { std::cout << err << std::endl; };

      VaultConfig config = VaultConfigBuilder()
        .withHost(Vault::Host{"localhost"})
        .withTlsEnabled(false)
        .build();

      return VaultClient{config, tokenStrategy, httpErrorCallback};
    }
  }
}

namespace KV {
  inline void setValues(KeyValue kv, const Vault::Path &path) {
    Parameters data(
      {
        {"foo",       "world"},
        {"baz",       "quux"},
        {"something", "something else"},
      });

    kv.put(path, data);
  }

  inline void assertListValues(KeyValue kv) {
    // TODO: Should probably refactor the list method, this doesn't make much sense
    auto response = kv.list(Vault::Path{""});

    if (response) {
      std::vector<std::string> secrets = nlohmann::json::parse(response.value())["data"]["keys"];

      CHECK(secrets.size() == 1);
      CHECK(secrets.at(0) == "hello");
    } else {
      CHECK(false);
    }
  }

  inline void assertGetValues(KeyValue kv, const Vault::Path &path) {
    auto response = kv.get(path);

    if (response) {
      std::unordered_map<std::string, std::string> secrets = nlohmann::json::parse(response.value())["data"]["data"];

      CHECK(secrets.size() == 3);

      auto baz = secrets.find("baz");
      CHECK(baz != secrets.end());
      CHECK(baz->second == "quux");
    } else {
      CHECK(false);
    }
  }

  inline void assertDestroyValues(KeyValue kv, const Vault::Path &path) {
    kv.del(path);

    CHECK(!kv.get(path));
  }
}

namespace TestHelpers::Transit {
  inline void assertHashEquals(const std::optional<std::string>& response, const std::string& expected) {
    if (response) {
      std::string actual = nlohmann::json::parse(response.value())["data"]["sum"];

      CHECK(expected == actual);
    } else {
      CHECK(false);
    }
  }
}