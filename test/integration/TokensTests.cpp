#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "VaultClient.h"
#include "TestHelpers.h"

#define SECONDS_IN_ONE_HOUR 3600

TEST_CASE("Token Authentication Tests")
{

  SECTION("Create Token")
  {
    Vault::Client root = TestHelpers::Token::login();

    // Create new token
    Vault::Tokens token{root};
    Vault::JsonParameters params {{"ttl", "1h"}, {"renewable", true}};
    params["meta"] = nlohmann::json{{"user", "armon"}};

    auto response = token.createToken(params);
    if (!response) CHECK(false);

    auto auth = nlohmann::json::parse(response.value())["auth"];
    CHECK(auth["lease_duration"] == SECONDS_IN_ONE_HOUR);
    CHECK(auth["metadata"]["user"] == "armon");
    CHECK(auth["renewable"] == true);

    Vault::Token new_token{auth["client_token"]};
    Vault::Client new_root{root, new_token};
    // Check if connection was actually created
    Vault::KeyValue kv{new_root};
    Vault::Path path{"hello"};
    KV::setValues(kv, path);
    KV::assertReadValues(kv, path);
  }
}