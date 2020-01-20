#include <catch2/catch.hpp>

#include "VaultClient.h"
#include "TestHelpers.h"

TEST_CASE("KeyValue Functions") {
  Vault::Client vaultClient = TestHelpers::AppRole::login();

  SECTION("v1") {
    Vault::SecretMount secretMount{"legacy"};
    Vault::KeyValue kv(vaultClient, secretMount, Vault::KeyValue::Version::v1);
    Vault::Path path("hello");

    KV::setValues(kv, path);
    KV::assertListValues(kv);
    KV::assertGetValues(kv, path);
    KV::assertDestroyValues(kv, path);
  }

  SECTION("v2") {
    Vault::KeyValue kv(vaultClient);
    Vault::Path path("hello");

    KV::setValues(kv, path);
    KV::assertListValues(kv);
    KV::assertGetValues(kv, path);
    KV::assertDestroyValues(kv, path);
  }
}
