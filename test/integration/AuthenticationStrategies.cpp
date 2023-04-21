#include <catch2/catch.hpp>

#include "TestHelpers.h"
#include "VaultClient.h"

TEST_CASE("Authentication Strategies") {
  SECTION("Token") {
    Vault::Client vaultClient = TestHelpers::Token::login();
    Vault::KeyValue kv(vaultClient);
    Vault::Path path("hello");

    KV::setValues(kv, path);
    KV::assertReadValues(kv, path);
  }

  SECTION("AppRole") {
    Vault::Client vaultClient = TestHelpers::AppRole::login();
    Vault::KeyValue kv(vaultClient);
    Vault::Path path("hello");

    KV::setValues(kv, path);
    KV::assertReadValues(kv, path);
  }
}
