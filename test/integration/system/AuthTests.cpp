#include <optional>
#include <catch2/catch.hpp>

#include "json.hpp"

#include "../TestHelpers.h"
#include "VaultClient.h"

TEST_CASE("Auth Functions") {
  Vault::Client vaultClient = TestHelpers::Token::login();
  Vault::Sys::Auth auth(vaultClient);

  SECTION("list") {
    auto first = auth.list();
    Vault::Parameters parameters{{"type", "userpass"}, {"description", "userpass"}};
    auth.enable(Vault::Path{"/userpass"}, parameters);
    auto userPass = auth.list();
    auth.disable(Vault::Path{"/userpass"});
    auto disabledUserPass = auth.list();

    if (first && userPass && disabledUserPass) {
      auto firstData = nlohmann::json::parse(first.value())["data"];
      CHECK(firstData.size() == 2);
      auto userPassData = nlohmann::json::parse(userPass.value())["data"];
      CHECK(userPassData.size() == 3);
      auto disabledUserPassData = nlohmann::json::parse(disabledUserPass.value())["data"];
      CHECK(disabledUserPassData.size() == 2);
    } else {
      CHECK(false);
    }
  }

  SECTION("readTuning") {
    Vault::Parameters parameters{{"max_lease_ttl", "5000"}};
    auth.tune(Vault::Path{"/approle"}, parameters);
    auto response = auth.readTuning(Vault::Path{"/approle"});

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      CHECK(data["force_no_cache"] == false);
      CHECK(data["token_type"] == "default-service");
      CHECK(data["max_lease_ttl"] == 5000);
    } else {
      CHECK(false);
    }
  }
}