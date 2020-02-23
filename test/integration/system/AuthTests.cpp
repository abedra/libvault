#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "VaultClient.h"
#include "../TestHelpers.h"

TEST_CASE("Auth Functions") {
  Vault::Client vaultClient = TestHelpers::Token::login();
  Vault::Sys::Auth auth(vaultClient);

  SECTION("list") {
    auto response = auth.list();

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      CHECK(data.size() == 2);
    } else {
      CHECK(false);
    }
  }

  SECTION("readTuning") {
    auto response = auth.readTuning(Vault::Path{"/approle"});

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      CHECK(data["force_no_cache"] == false);
      CHECK(data["token_type"] == "default-service");
    } else {
      CHECK(false);
    }
  }
}