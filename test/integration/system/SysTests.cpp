#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "../TestHelpers.h"
#include "VaultClient.h"

TEST_CASE("Sys Functions") {
  Vault::Client vaultClient = TestHelpers::AppRole::login();
  Vault::Sys sys(vaultClient);

  SECTION("health") {
    auto response = sys.health();

    if (response) {
      auto keys = nlohmann::json::parse(response.value());

      REQUIRE(keys["initialized"] == true);
      REQUIRE(keys["sealed"] == false);
    } else {
      REQUIRE(false);
    }
  }

  SECTION("health with supplied leader") {
    auto response = sys.health(Vault::Url{"http://localhost:8200"});

    if (response) {
      auto keys = nlohmann::json::parse(response.value());

      REQUIRE(keys["initialized"] == true);
      REQUIRE(keys["sealed"] == false);
    } else {
      REQUIRE(false);
    }
  }

  SECTION("leader") {
    auto response = sys.leader();

    if (response) {
      auto keys = nlohmann::json::parse(response.value());

      REQUIRE(keys["ha_enabled"] == false);
      REQUIRE(keys["leader"].empty());
    } else {
      REQUIRE(false);
    }
  }

  SECTION("wrap") {
    Vault::Parameters parameters({{"foo", "bar"}});
    auto response = sys.wrap(parameters, Vault::TTL{300});

    if (response) {
      auto wrap_info = nlohmann::json::parse(response.value())["wrap_info"];
      REQUIRE(!wrap_info["token"].empty());
      REQUIRE(!wrap_info["accessor"].empty());
      REQUIRE(wrap_info["ttl"] == 300);
    } else {
      REQUIRE(false);
    }
  }

  SECTION("lookup") {
    Vault::Parameters parameters({{"foo", "bar"}});
    auto wrapped = sys.wrap(parameters, Vault::TTL{300});

    if (wrapped) {
      auto wrap_info = nlohmann::json::parse(wrapped.value())["wrap_info"];
      auto token = Vault::Token{wrap_info["token"]};
      auto lookup = sys.lookup(token);

      if (lookup) {
        auto data = nlohmann::json::parse(lookup.value())["data"];
        REQUIRE(data["creation_ttl"] == 300);
        REQUIRE(data["creation_path"] == "sys/wrapping/wrap");
      } else {
        REQUIRE(false);
      }
    } else {
      REQUIRE(false);
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
        REQUIRE(data["foo"] == "bar");
      } else {
        REQUIRE(false);
      }
    } else {
      REQUIRE(false);
    }
  }

  SECTION("rewrap") {
    Vault::Parameters parameters({{"foo", "bar"}});
    auto wrapped = sys.wrap(parameters, Vault::TTL{300});

    if (wrapped) {
      auto wrap_info = nlohmann::json::parse(wrapped.value())["wrap_info"];
      auto token = Vault::Token{wrap_info["token"]};
      auto rewrapped = sys.rewrap(token);

      if (rewrapped) {
        auto rewrap_info =
            nlohmann::json::parse(rewrapped.value())["wrap_info"];
        REQUIRE(wrap_info["token"] != rewrap_info["token"]);
        REQUIRE(!rewrap_info["token"].empty());
        REQUIRE(!rewrap_info["accessor"].empty());
        REQUIRE(rewrap_info["ttl"] == 300);
      } else {
        REQUIRE(false);
      }
    } else {
      REQUIRE(false);
    }
  }
}
