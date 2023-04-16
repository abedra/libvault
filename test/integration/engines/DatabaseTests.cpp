#include <catch2/catch.hpp>

#include "../TestHelpers.h"
#include "VaultClient.h"

TEST_CASE("PostgreSQL") {
  Vault::Client vaultClient(TestHelpers::AppRole::login());
  Vault::Database postgres(vaultClient);
  Vault::Path path{"postgresql"};
  Vault::Parameters configuration(
      {{"plugin_name", "postgresql-database-plugin"},
       {"allowed_roles", "readonly"},
       {"connection_url",
        "postgresql://postgres:postgres@docker-postgres-1:5432/"
        "vault?sslmode=disable"}});

  Vault::Parameters role(
      {{"db_name", "postgresql"},
       {"default_ttl", "1h"},
       {"max_ttl", "24h"},
       {"creation_statements",
        R"(CREATE ROLE "{{name}}" WITH LOGIN PASSWORD '{{password}}' VALID UNTIL '{{expiration}}'; GRANT SELECT ON ALL TABLES IN SCHEMA public TO "{{name}}";)"}});

  Vault::Path rolePath{"readonly"};
  postgres.configureConnection(path, configuration);
  postgres.deleteRole(rolePath);
  postgres.createRole(rolePath, role);

  SECTION("Generate Credentials") {
    auto response = postgres.generateCredentials(rolePath);

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      REQUIRE(!data["username"].empty());
      REQUIRE(!data["password"].empty());
    } else {
      REQUIRE(false);
    }
  }
}
