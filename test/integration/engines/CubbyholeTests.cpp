#include <catch2/catch.hpp>

#include "VaultClient.h"
#include "../TestHelpers.h"

TEST_CASE("Cubbyhole")
{
  Vault::Client vaultClient = TestHelpers::AppRole::login();
  Vault::Cubbyhole cubbyhole(vaultClient);
  Vault::Path path("mykey");
  Vault::Parameters parameters({
    {"foo", "bar"},
    {"zip", "zap"}
  });
  cubbyhole.create(path, parameters);

  SECTION("read")
  {
    auto response = cubbyhole.read(path);

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      CHECK(data["foo"] == "bar");
      CHECK(data["zip"] == "zap");
    } else {
      CHECK(false);
    }
  }

  SECTION("del")
  {
    Vault::Path throwaway("throwaway");
    Vault::Parameters throwawayParameters({{"one", "two"}});
    cubbyhole.create(throwaway, throwawayParameters);

    auto before = cubbyhole.list(Vault::Path{""});

    if (before) {
      auto keys = nlohmann::json::parse(before.value())["data"]["keys"];
      CHECK(keys.size() == 2);
      CHECK(keys.at(0) == "mykey");
      CHECK(keys.at(1) == "throwaway");

      cubbyhole.del(throwaway);

      auto after = cubbyhole.list(Vault::Path{""});

      if (after) {
        keys = nlohmann::json::parse(after.value())["data"]["keys"];
        CHECK(keys.size() == 1);
        CHECK(keys.at(0) == "mykey");
      } else {
        CHECK(false);
      }
    } else {
      CHECK(false);
    }
  }
}
