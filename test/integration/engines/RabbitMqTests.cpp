#include <catch2/catch.hpp>

#include "../TestHelpers.h"
#include "VaultClient.h"

TEST_CASE("RabbitMQ") {
  Vault::Client vaultClient = TestHelpers::AppRole::login();
  Vault::RabbitMq rabbitMq(vaultClient);
  Vault::Parameters configuration(
      {{"connection_uri", "http://docker-rabbitmq-1:15672"},
       {"username", "guest"},
       {"password", "guest"}});

  Vault::Parameters role(
      {{"vhosts", R"({"/":{"write": ".*", "read": ".*", "configure": ".*"}})"},
       {"tags", "administrator"}});

  const Vault::Path &path = Vault::Path{"my-role"};

  rabbitMq.configureConnection(configuration);
  rabbitMq.deleteRole(path);
  rabbitMq.createRole(path, role);

  SECTION("Generate Configuration") {
    auto response = rabbitMq.generateCredentials(path);

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      REQUIRE(!data["username"].empty());
      REQUIRE(!data["password"].empty());
    } else {
      REQUIRE(false);
    }
  }
}
