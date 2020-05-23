#include <catch2/catch.hpp>

#include "VaultClient.h"
#include "../TestHelpers.h"

TEST_CASE("RabbitMQ") {
  Vault::Client vaultClient = TestHelpers::AppRole::login();
  Vault::RabbitMq rabbitMq(vaultClient);
  Vault::Parameters configuration({
    {"connection_uri", "http://localhost:15672"},
    {"username", "guest"},
    {"password", "guest"}
  });

  Vault::Parameters role({
    {"vhosts", R"({"/":{"write": ".*", "read": ".*", "configure": ".*"}})"},
    {"tags", "administrator"}
  });

  const Vault::Path &path = Vault::Path{"my-role"};

  rabbitMq.configureConnection(configuration);
  rabbitMq.deleteRole(path);
  rabbitMq.createRole(path, role);
  
  SECTION("Generate Configuration")
  {
    auto response = rabbitMq.generateCredentials(path);

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      std::cout << data << std::endl;
      CHECK(!data["username"].empty());
      CHECK(!data["password"].empty());
    } else {
      CHECK(false);
    }
  }
}