#include <catch2/catch.hpp>

#include "VaultClient.h"
#include "TestHelpers.h"

TEST_CASE("Authentication Strategies")
{
  SECTION("Token")
  {
    VaultClient vaultClient = TestHelpers::Token::login();
    KeyValue kv(vaultClient);
    Vault::Path path("hello");

    KV::setValues(kv, path);
    KV::assertGetValues(kv, path);
  }

  SECTION("AppRole")
  {
    VaultClient vaultClient = TestHelpers::AppRole::login();
    KeyValue kv(vaultClient);
    Vault::Path path("hello");

    KV::setValues(kv, path);
    KV::assertGetValues(kv, path);
  }
}