#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "VaultClient.h"
#include "TestSetup.h"

TEST_CASE("KeyValue Functions") {
  VaultClient vaultClient = login();

  SECTION("v2") {
    KeyValue kv(vaultClient);
    Vault::Path path("hello");

    Parameters data(
        {
            {"foo","world"},
            {"baz","quux"},
            {"something", "something else"},
        });

    kv.put(path, data);
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
}
