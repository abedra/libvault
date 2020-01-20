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
  namespace Token {
    inline Vault::Client login() {
      Vault::Token token{getOrDefault("VAULT_SINGLE_TOKEN", "")};
      Vault::TokenStrategy tokenStrategy{token};
      Vault::HttpErrorCallback httpErrorCallback = [&](const std::string &err) { std::cout << err << std::endl; };

      Vault::Config config = Vault::ConfigBuilder()
        .withHost(Vault::Host{"localhost"})
        .withTlsEnabled(false)
        .build();

      return Vault::Client{config, tokenStrategy, httpErrorCallback};
    }
  }

  namespace AppRole {
    inline Vault::Client login() {
      Vault::RoleId roleId{getOrDefault("APPROLE_ROLE_ID", "")};
      Vault::SecretId secretId{getOrDefault("APPROLE_SECRET_ID", "")};
      Vault::AppRoleStrategy authStrategy{roleId, secretId};
      Vault::HttpErrorCallback httpErrorCallback = [&](const std::string &err) { std::cout << err << std::endl; };

      Vault::Config config = Vault::ConfigBuilder()
        .withHost(Vault::Host{"localhost"})
        .withTlsEnabled(false)
        .withDebug(false)
        .build();

      return Vault::Client{config, authStrategy, httpErrorCallback};
    }
  }

  namespace WrappedAppRole {
    inline Vault::Client login() {
      Vault::RoleId roleId{getOrDefault("APPROLE_ROLE_ID", "")};
      Vault::Token wrappedToken{getOrDefault("APPROLE_WRAPPED_TOKEN", "")};
      Vault::WrappedSecretAppRoleStrategy wrappedAuthStrategy{roleId, wrappedToken};
      Vault::HttpErrorCallback httpErrorCallback = [&](const std::string &err) { std::cout << err << std::endl; };
      Vault::Config config = Vault::ConfigBuilder()
        .withHost(Vault::Host{"localhost"})
        .withTlsEnabled(false)
        .withDebug(false)
        .build();

      return Vault::Client{config, wrappedAuthStrategy, httpErrorCallback};
    }
  }

  namespace Ldap {
    inline Vault::Client login() {
      auto username = getOrDefault("LDAP_USERNAME", "");
      auto password = getOrDefault("LDAP_PASSWORD", "");
      auto ldapStrategy = Vault::LdapStrategy{username, password};
      Vault::HttpErrorCallback httpErrorCallback = [&](const std::string &err) { std::cout << err << std::endl; };
      Vault::Config config = Vault::ConfigBuilder()
        .withHost(Vault::Host{"localhost"})
        .withTlsEnabled(false)
        .withDebug(false)
        .build();

      return Vault::Client{config, ldapStrategy, httpErrorCallback};
    }
  }
}

namespace KV {
  inline void setValues(Vault::KeyValue kv, const Vault::Path &path) {
    Vault::Parameters data(
      {
        {"foo",       "world"},
        {"baz",       "quux"},
        {"something", "something else"},
      });

    kv.create(path, data);
  }

  inline void assertListValues(Vault::KeyValue kv) {
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

  inline void assertReadValues(Vault::KeyValue kv, const Vault::Path &path) {
    auto response = kv.read(path);

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

  inline void assertDeleteValues(Vault::KeyValue kv, const Vault::Path &path) {
    kv.del(path);

    CHECK(!kv.read(path));
  }
}

namespace TestHelpers::Transit {
  inline void assertHashPopulated(const std::optional<std::string>& response) {
    if (response) {
      std::string actual = nlohmann::json::parse(response.value())["data"]["sum"];

      CHECK(!actual.empty());
    } else {
      CHECK(false);
    }
  }

  inline void assertHmacPopulated(const std::optional<std::string>& response) {
    if (response) {
      std::string actual = nlohmann::json::parse(response.value())["data"]["hmac"];

      CHECK(!actual.empty());
    } else {
      CHECK(false);
    }
  }

  inline void assertSignaturePopulated(const std::optional<std::string>& response) {
    if (response) {
      std::string actual = nlohmann::json::parse(response.value())["data"]["signature"];

      CHECK(!actual.empty());
    } else {
      CHECK(false);
    }
  }
}