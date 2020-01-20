#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "VaultClient.h"
#include "TestHelpers.h"

TEST_CASE("AppRole Management Functions")
{
  Vault::Client vaultClient = TestHelpers::AppRole::login();

  SECTION("list")
  {
      Vault::AppRole appRole(vaultClient);
      auto response = appRole.list();

      if (response) {
        std::vector<std::string> keys = nlohmann::json::parse(response.value())["data"]["keys"];

        CHECK(keys.size() == 1);
        CHECK(keys.at(0) == "client");
      } else {
        CHECK(false);
      }
  }
}
