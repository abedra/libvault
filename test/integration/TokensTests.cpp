#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "VaultClient.h"
#include "TestHelpers.h"

#define SECONDS_IN_ONE_HOUR 3600

TEST_CASE("Token Authentication Tests")
{
  Vault::Client root = TestHelpers::Token::login();
  Vault::Tokens token{root};

  SECTION("Create Token")
  {
    // Create new token
    Vault::JsonParameters params{{"ttl",       "1h"},
                                 {"renewable", true}};
    params["meta"] = nlohmann::json{{"user", "armon"}};

    auto response = token.createToken(params);
    REQUIRE(response.has_value());

    auto auth = nlohmann::json::parse(response.value())["auth"];
    CHECK(auth["lease_duration"] == SECONDS_IN_ONE_HOUR);
    CHECK(auth["metadata"]["user"] == "armon");
    CHECK(auth["renewable"] == true);

    Vault::Token new_token{auth["client_token"]};
    Vault::Client new_root{root, new_token};

    SECTION("Check if token can read/write KV") {
      Vault::KeyValue kv{new_root};
      Vault::Path path{"hello"};
      KV::setValues(kv, path);
      KV::assertReadValues(kv, path);
    }

    SECTION("LookupTokenSelf") {
      Vault::Tokens new_token{new_root};
      auto response = new_token.lookupTokenSelf();
      auto data = nlohmann::json::parse(response.value())["data"];
      REQUIRE(response.has_value());
      CHECK(data["id"] == new_root.getToken().value());
      CHECK(data["creation_ttl"] == SECONDS_IN_ONE_HOUR);
    }

    SECTION("RevokeTokenSelf") {
      // Get accessor
      Vault::Tokens new_token{new_root};
      auto token_info = new_token.lookupTokenSelf();
      REQUIRE(token_info.has_value());
      std::string accessor = nlohmann::json::parse(token_info.value())["data"]["accessor"];

      // Get list of tokens and check that token not in list
      new_token.revokeTokenSelf();
      response = token.listAccessors();
      REQUIRE(response.has_value());
      std::vector<std::string> accessor_list = nlohmann::json::parse(response.value())["data"]["keys"];
      CHECK(std::find(accessor_list.begin(), accessor_list.end(), accessor) == accessor_list.end());
    }
  }

  SECTION("ListAccessors") {
    // get the token's accessor
    auto token_info = token.lookupTokenSelf();
    REQUIRE(token_info.has_value());
    std::string accessor = nlohmann::json::parse(token_info.value())["data"]["accessor"];

    // check if accessor is in the list
    auto response = token.listAccessors();
    REQUIRE(response.has_value());
    std::vector<std::string> accessor_list = nlohmann::json::parse(response.value())["data"]["keys"];
    CHECK(std::find(accessor_list.begin(), accessor_list.end(), accessor) != accessor_list.end());
  }
}