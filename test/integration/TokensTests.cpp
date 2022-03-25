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

  SECTION("Create Token and Check Self Functions")
  {
    // Create new token
    Vault::Parameters params{{"ttl",       "1h"},
                             {"renewable", "true"},
                             {"meta", Vault::Map{{"user", "Armon"}}}};

    auto response = token.createToken(params);
    REQUIRE(response.has_value());

    auto auth = nlohmann::json::parse(response.value())["auth"];
    CHECK(auth["lease_duration"] == SECONDS_IN_ONE_HOUR);
    CHECK(auth["renewable"] == true);

    Vault::Token new_token{auth["client_token"]};
    Vault::Client new_root{root, new_token};
    Vault::Tokens new_tokens{new_root};

    SECTION("Check if token can read/write KV") {
      Vault::KeyValue kv{new_root};
      Vault::Path path{"hello"};
      KV::setValues(kv, path);
      KV::assertReadValues(kv, path);
    }

    SECTION("LookupTokenSelf") {
      auto lookup_response = new_tokens.lookupTokenSelf();
      REQUIRE(lookup_response.has_value());

      auto data = nlohmann::json::parse(lookup_response.value())["data"];
      CHECK(data["id"] == new_root.getToken().value());
      CHECK(data["creation_ttl"] == SECONDS_IN_ONE_HOUR);
    }

    SECTION("RenewTokenSelf") {
      Vault::Parameters renew_params {{"increment", "2h"}};
      auto renew_response = new_tokens.renewTokenSelf(renew_params);
      REQUIRE(renew_response.has_value());
      auto new_ttl = nlohmann::json::parse(renew_response.value())["auth"]["lease_duration"];
      CHECK(new_ttl == 2 * SECONDS_IN_ONE_HOUR);
    }

    SECTION("RevokeTokenSelf") {
      // Get accessor
      auto token_info = new_tokens.lookupTokenSelf();
      REQUIRE(token_info.has_value());
      std::string accessor = nlohmann::json::parse(token_info.value())["data"]["accessor"];

      // Get list of tokens and check that token not in list
      new_tokens.revokeTokenSelf();
      response = token.listAccessors();
      REQUIRE(response.has_value());
      std::vector<std::string> accessor_list = nlohmann::json::parse(response.value())["data"]["keys"];
      CHECK(std::find(accessor_list.begin(), accessor_list.end(), accessor) == accessor_list.end());
    }
  }

  SECTION("Create Token and Check Accessor Based Functions")
  {
    // Create new token
    Vault::Parameters params{{"ttl",       "1h"},
                                 {"renewable", "true"}};
    auto response = token.createToken(params);
    REQUIRE(response.has_value());
    std::string token_accessor = nlohmann::json::parse(response.value())["auth"]["accessor"];

    SECTION("LookupToken") {
      Vault::Parameters lookup_params{{"accessor", token_accessor}};
      auto lookup_response = token.lookupTokenAccessor(lookup_params);
      CHECK(lookup_response.has_value());
    }

    SECTION("RenewToken") {
      Vault::Parameters renew_params{{"accessor", token_accessor},
                               {"increment", "2h"}};
      auto renew_response = token.renewTokenAccessor(renew_params);
      REQUIRE(response.has_value());
      auto new_ttl = nlohmann::json::parse(renew_response.value())["auth"]["lease_duration"];
      CHECK(new_ttl == 2 * SECONDS_IN_ONE_HOUR);
    }

    SECTION("RevokeToken") {
      Vault::Parameters revoke_params{{"accessor", token_accessor}};
      auto revoke_response = token.revokeTokenAccessor(revoke_params);
      auto lookup_response = token.lookupTokenAccessor(revoke_params);
      CHECK(!lookup_response.has_value());
    }
  }

  SECTION("Create Token and Check Token Based Functions")
  {
    // Create new token
    Vault::Parameters params{{"ttl",       "1h"},
                                 {"renewable", "true"}};
    auto response = token.createToken(params);
    REQUIRE(response.has_value());
    std::string new_token = nlohmann::json::parse(response.value())["auth"]["client_token"];

    SECTION("LookupToken") {
      Vault::Parameters lookup_params{{"token", new_token}};
      auto lookup_response = token.lookupToken(lookup_params);
      CHECK(lookup_response.has_value());
    }

    SECTION("RenewToken") {
      Vault::Parameters renew_params{{"token", new_token},
                               {"increment", "2h"}};
      auto renew_response = token.renewToken(renew_params);
      REQUIRE(response.has_value());
      auto new_ttl = nlohmann::json::parse(renew_response.value())["auth"]["lease_duration"];
      CHECK(new_ttl == 2 * SECONDS_IN_ONE_HOUR);
    }

    SECTION("RevokeToken") {
      Vault::Parameters revoke_params{{"token", new_token}};
      auto revoke_response = token.revokeToken(revoke_params);
      auto lookup_response = token.lookupToken(revoke_params);
      CHECK(!lookup_response.has_value());
    }
  }

  SECTION("Create Orphan Token") {
    Vault::Parameters params{{"policies",  "default"},
                             {"ttl",       "1h"},
                             {"renewable", "true"}};
    auto response = token.createTokenOrphan(params);
    REQUIRE(response.has_value());
    bool orphan = nlohmann::json::parse(response.value())["auth"]["orphan"];
    CHECK(orphan);
  }

  SECTION("Revoke Token and Orphan Children") {
    Vault::Parameters params{{"ttl", "1h"}};

    // Create a new token and a client based on it
    auto create_response = token.createToken(params);
    REQUIRE(create_response.has_value());
    std::string child_token = nlohmann::json::parse(create_response.value())["auth"]["client_token"];
    Vault::Token new_token{child_token};
    Vault::Client child_client{root, new_token};
    Vault::Tokens child_tokens{child_client};

    // Create children of children
    create_response = child_tokens.createToken(params);
    REQUIRE(create_response.has_value());
    std::string child_child_token = nlohmann::json::parse(create_response.value())["auth"]["client_token"];

    // Delete parent and check child is alive
    Vault::Parameters token_to_be_deleted{{"token", child_token}};
    Vault::Parameters token_to_stay_alive{{"token", child_child_token}};
    token.revokeTokenAndOrphanChildren(token_to_be_deleted);
    CHECK_FALSE(token.lookupToken(token_to_be_deleted).has_value());
    CHECK(token.lookupToken(token_to_stay_alive).has_value());
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

  SECTION("Role specific tests") {
    Vault::Path role1{"role1"};
    token.createTokenRole(role1, {});
    Vault::Path role2{"role2"};
    token.createTokenRole(role2, {});


    SECTION("List Role") {
      auto token_roles = token.listTokenRoles();
      REQUIRE(token_roles.has_value());
      auto roles_list = nlohmann::json::parse(token_roles.value())["data"]["keys"];
      CHECK(std::find(roles_list.begin(), roles_list.end(), role1.value()) != roles_list.end());
    }

    SECTION("Create Token With Role") {
      Vault::Parameters params{{"ttl",       "1h"}};
      auto token_response = token.createTokenWithRole(role1, params);
      REQUIRE(token_response.has_value());

      std::string new_token = nlohmann::json::parse(token_response.value())["auth"]["client_token"];
      auto res = token.lookupToken(Vault::Parameters{{"token", new_token}});
      std::filesystem::path path = nlohmann::json::parse(*res)["data"]["path"];
      CHECK(path.filename() == role1.value());
    }

    SECTION("ReadRole") {
      auto response = token.readTokenRole(role1);
      CHECK(response.has_value());
    }

    SECTION("DeleteRole") {
      token.deleteTokenRole(role1);
      auto token_roles = token.listTokenRoles();
      auto roles_list = nlohmann::json::parse(token_roles.value())["data"]["keys"];
      CHECK(std::find(roles_list.begin(), roles_list.end(), role2.value()) != roles_list.end());
    }
  }

  SECTION("TidyTokens") {
    auto response = token.tidyTokens();
    CHECK(response.has_value());
  }

  SECTION("WrappedTokenCreation") {
    Vault::Parameters params{{"ttl", "1h"},
                                 {"renewable", "true"}};

    Vault::TTL ttl{60};
    auto response = token.createWrappedToken(params, ttl);
    CHECK(response.has_value());
    auto wrapped_token = nlohmann::json::parse(response.value())["wrap_info"]["token"];

    // create client with wrapping token
    Vault::Token new_token{wrapped_token};
    Vault::Client new_root{root, new_token};
    Vault::Sys sys{new_root};

    // Check token health
    auto resp = sys.lookup(new_root.getToken());
    CHECK(resp.has_value());

    // Receive token
    resp = sys.unwrap(new_root.getToken());
    CHECK(resp.has_value());
  }
}