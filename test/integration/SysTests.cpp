#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "VaultClient.h"
#include "TestHelpers.h"

TEST_CASE("Sys Functions")
{
  Vault::Client vaultClient = TestHelpers::AppRole::login();

  SECTION("health")
  {
    Vault::Sys sys(vaultClient);
    auto response = sys.health();

    if (response) {
      auto keys = nlohmann::json::parse(response.value());

      CHECK(keys["initialized"] == true);
      CHECK(keys["sealed"] == false);
    } else {
      CHECK(false);
    }
  }

  SECTION("health with supplied leader")
  {
    Vault::Sys sys(vaultClient);
    auto response = sys.health(Vault::Path{"http://localhost:8200"});

    if (response) {
      auto keys = nlohmann::json::parse(response.value());

      CHECK(keys["initialized"] == true);
      CHECK(keys["sealed"] == false);
    } else {
      CHECK(false);
    }
  }

  SECTION("leader")
  {
    Vault::Sys sys(vaultClient);
    auto response = sys.leader();

    if (response) {
      auto keys = nlohmann::json::parse(response.value());

      CHECK(keys["ha_enabled"] == false);
      CHECK(keys["leader"].empty());
    } else {
      CHECK(false);
    }
  }
}

