#include <catch2/catch.hpp>

#include "VaultClient.h"
#include "../TestHelpers.h"

TEST_CASE("Totp")
{
  Vault::Client vaultClient = TestHelpers::AppRole::login();
  Vault::Totp totp(vaultClient);
  Vault::Path path("mykey");
  Vault::Parameters parameters({
    {"url", "otpauth://totp/Google:test@gmail.com?secret=Y64VEVMBTSXCYIWRSHRNDZW62MPGVU2G&issuer=Google"},
    {"generate", "false"}
  });
  totp.create(path, parameters);

  SECTION("list")
  {
    auto response = totp.list();

    if (response) {
      auto keys = nlohmann::json::parse(response.value())["data"]["keys"];
      CHECK(keys.size() == 1);
      CHECK(keys.at(0) == "mykey");
    } else {
      CHECK(false);
    }
  }

  SECTION("read")
  {
    auto response = totp.read(path);

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      CHECK(data["account_name"] == "test@gmail.com");
      CHECK(data["algorithm"] == "SHA1");
      CHECK(data["digits"] == 6);
      CHECK(data["issuer"] == "Google");
      CHECK(data["period"] == 30);
    } else {
      CHECK(false);
    }
  }

  SECTION("generate/validate")
  {
    auto generate = totp.generate(path);

    if (generate) {
      auto code = nlohmann::json::parse(generate.value())["data"]["code"];
      CHECK(!code.empty());

      auto validate = totp.validate(path, Vault::Parameters({{"code", std::string(code)}}));

      if (validate) {
        auto valid = nlohmann::json::parse(validate.value())["data"]["valid"];
        CHECK(valid);
      } else {
        CHECK(false);
      }
    } else {
      CHECK(false);
    }
  }

  SECTION("del")
  {
    Vault::Path throwaway{"throwaway"};
    Vault::Parameters throwawayParameters({
      {"url", "otpauth://totp/Google:test@gmail.com?secret=Y64VEVMBTSXCYIWRSHRNDZW62MPGVU2G&issuer=Google"},
      {"generate", "false"}
    });
    totp.create(throwaway, throwawayParameters);

    auto withThrowaway = totp.list();

    if (withThrowaway) {
      auto keys = nlohmann::json::parse(withThrowaway.value())["data"]["keys"];
      CHECK(keys.size() == 2);
      CHECK(keys.at(0) == "mykey");
      CHECK(keys.at(1) == "throwaway");

      totp.del(throwaway);
      auto withoutThrowaway = totp.list();

      if (withThrowaway) {
        keys = nlohmann::json::parse(withoutThrowaway.value())["data"]["keys"];
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
