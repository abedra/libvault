#include <catch2/catch.hpp>

#include "../TestHelpers.h"
#include "VaultClient.h"

TEST_CASE("KeyValue Functions") {
  Vault::Client vaultClient = TestHelpers::AppRole::login();

  SECTION("v1") {
    Vault::SecretMount secretMount{"legacy"};
    Vault::KeyValue kv(vaultClient, secretMount, Vault::KeyValue::Version::v1);
    Vault::Path path("hello");
    std::vector<int64_t> versions({1, 2, 3});

    SECTION("CRUD") {
      KV::setValues(kv, path);
      KV::assertListValues(kv);
      KV::assertReadValues(kv, path);
      KV::assertDeleteValues(kv, path);
    }

    SECTION("v2 only") {
      std::vector<std::optional<std::string>> responses;
      responses.push_back(kv.readConfig());
      responses.push_back(kv.updateConfig({}));
      responses.push_back(kv.readMetadata(path));
      responses.push_back(kv.updateMetadata(path, {}));
      responses.push_back(kv.deleteMetadata(path));
      responses.push_back(kv.del(path, versions));
      responses.push_back(kv.destroy(path, versions));
      responses.push_back(kv.undelete(path, versions));

      for (auto &response : responses) {
        REQUIRE(!response);
      }
    }
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

    SECTION("Read previous version") {
      Vault::Parameters data({
          {"foo", "updated"},
          {"baz", "updated"},
          {"something", "updated"},
      });

      kv.create(path, data);

      auto response = kv.read(path, Vault::SecretVersion{1});

      if (response) {
        std::unordered_map<std::string, std::string> secrets =
            nlohmann::json::parse(response.value())["data"]["data"];

        REQUIRE(secrets.size() == 3);

        auto baz = secrets.find("baz");
        REQUIRE(baz != secrets.end());
        REQUIRE(baz->second == "quux");
      } else {
        REQUIRE(false);
      }
    }

    SECTION("v1 only") {
      auto response = kv.update(path, {});
      REQUIRE(!response);
    }

    SECTION("config") {
      Vault::Parameters parameters({{"max_versions", "10"}});
      kv.updateConfig(parameters);

      auto response = kv.readConfig();
      if (response) {
        auto config = nlohmann::json::parse(response.value())["data"];

        REQUIRE(config["cas_required"] == false);
        REQUIRE(config["max_versions"] == 10);
      } else {
        REQUIRE(false);
      }
    }

    SECTION("metadata") {
      Vault::Parameters parameters({{"max_versions", "10"}});
      kv.updateMetadata(path, parameters);

      auto response = kv.readMetadata(path);
      if (response) {
        auto metadata = nlohmann::json::parse(response.value())["data"];

        REQUIRE(metadata["current_version"] > 0);
        REQUIRE(metadata["max_versions"] == 10);

        kv.deleteMetadata(path);
        response = kv.readMetadata(path);
        REQUIRE(!response);
      } else {
        REQUIRE(false);
      }
    }
  }
}
