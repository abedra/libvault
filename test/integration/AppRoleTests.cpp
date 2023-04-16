#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "TestHelpers.h"
#include "VaultClient.h"

TEST_CASE("AppRole Management Functions") {
  Vault::Client vaultClient = TestHelpers::AppRole::login();

  SECTION("list") {
    Vault::AppRole appRole(vaultClient);
    auto response = appRole.list();

    if (response) {
      std::vector<std::string> keys =
          nlohmann::json::parse(response.value())["data"]["keys"];

      REQUIRE(keys.size() == 1);
      REQUIRE(keys.at(0) == "client");
    } else {
      REQUIRE(false);
    }
  }
}
