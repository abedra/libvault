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
    KV::assertReadValues(kv, path);
    KV::assertDeleteValues(kv, path);
  }

  SECTION("v2") {
    Vault::KeyValue kv(vaultClient);
    Vault::Path path("hello");

    SECTION("CRUD") {
      KV::setValues(kv, path);
      KV::assertListValues(kv);
      KV::assertReadValues(kv, path);
      KV::assertDeleteValues(kv, path);
    }

    SECTION("config") {
      Vault::Parameters parameters({ {"max_versions", "10"} });
      kv.updateConfig(parameters);

      auto response = kv.readConfig();
      if (response) {
        auto config = nlohmann::json::parse(response.value())["data"];

        CHECK(config.size() == 2);
        CHECK(config["cas_required"] == false);
        CHECK(config["max_versions"] == 10);
      } else {
        CHECK(false);
      }
    }
  }
}
