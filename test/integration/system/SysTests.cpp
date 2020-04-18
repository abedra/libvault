#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "VaultClient.h"
#include "../TestHelpers.h"

TEST_CASE("Sys Functions")
{
  Vault::Client vaultClient = TestHelpers::AppRole::login();
  Vault::Sys sys(vaultClient);

  SECTION("health")
  {
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
    auto response = sys.leader();

    if (response) {
      auto keys = nlohmann::json::parse(response.value());

      CHECK(keys["ha_enabled"] == false);
      CHECK(keys["leader"].empty());
    } else {
      CHECK(false);
    }
  }

  SECTION("wrap")
  {
    Vault::Parameters parameters({{"foo", "bar"}});
    auto response = sys.wrap(parameters, Vault::TTL{300});

    if (response) {
      auto wrap_info = nlohmann::json::parse(response.value())["wrap_info"];
      CHECK(!wrap_info["token"].empty());
      CHECK(!wrap_info["accessor"].empty());
      CHECK(wrap_info["ttl"] == 300);
    } else {
      CHECK(false);
    }
  }

  SECTION("unwrap") {
    Vault::Parameters parameters({{"foo", "bar"}});
    auto wrapped = sys.wrap(parameters, Vault::TTL{300});

    if (wrapped) {
      auto wrap_info = nlohmann::json::parse(wrapped.value())["wrap_info"];
      auto token = Vault::Token{wrap_info["token"]};
      auto unwrapClient = Vault::Client(vaultClient, token);
      auto unwrapped = sys.unwrap(token);

      if (unwrapped) {
        auto data = nlohmann::json::parse(unwrapped.value())["data"];
        CHECK(data["foo"] == "bar");
      } else {
        CHECK(false);
      }
    } else {
      CHECK(false);
    }
  }
}

