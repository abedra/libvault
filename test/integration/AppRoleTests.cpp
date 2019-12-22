#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "VaultClient.h"

template<typename T>
void print_response(std::optional<T> response) {
  if (response) {
    std::cout << response.value() << std::endl;
  } else {
    std::cout << "Error" << std::endl;
  }
}

static std::string getOrDefault(const char *name, const std::string& defaultValue) {
  auto value = std::getenv(name);

  return value ? value : defaultValue;
}

static std::unordered_map<std::string, std::string> responseToMap(const std::optional<std::string>& response) {
  return nlohmann::json::parse(response.value())["data"];
}

TEST_CASE("AppRole Management Functions")
{
  Vault::RoleId roleId{getOrDefault("APPROLE_ROLE_ID", "")};
  Vault::SecretId secretId{getOrDefault("APPROLE_SECRET_ID", "")};
  AppRoleStrategy authStrategy{roleId, secretId};
  HttpErrorCallback httpErrorCallback = [&](const std::string& err) { std::cout << err << std::endl; };
  VaultConfig config = VaultConfigBuilder()
      .withHost(Vault::Host{"localhost"})
      .withTlsEnabled(false)
      .build();

  VaultClient vaultClient{config, authStrategy, httpErrorCallback};

  SECTION("list")
  {
      AppRole appRole(vaultClient);
      auto response = appRole.list();

      if (response) {
        std::vector<std::string> keys = nlohmann::json::parse(response.value())["data"]["keys"];

        CHECK(keys.size() == 1);
        CHECK(keys.at(0) == "client");
      } else {
        CHECK(false);
      }
  }
}
