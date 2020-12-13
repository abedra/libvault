#include <iostream>
#include <filesystem>
#include <fstream>
#include "../../../../lib/json.hpp"
#include "VaultClient.h"

struct Database {
  std::string host;
  int port;
  std::string name;
  std::string username;
  std::string password;

  Database withSecrets(const Vault::Client &vaultClient) {
    Vault::KeyValue kv{vaultClient};
    auto databaseSecrets = kv.read(Vault::Path{"database"});
    if (databaseSecrets) {
      std::unordered_map<std::string, std::string> secrets = nlohmann::json::parse(databaseSecrets.value())["data"]["data"];
      this->username = secrets.at(this->username);
      this->password = secrets.at(this->password);

      return *this;
    } else {
      return *this;
    }
  }
};

void from_json(const nlohmann::json &j, Database &database) {
  j.at("host").get_to(database.host);
  j.at("port").get_to(database.port);
  j.at("name").get_to(database.name);
  j.at("username").get_to(database.username);
  j.at("password").get_to(database.password);
}

Database getDatabaseConfiguration(const std::filesystem::path &path) {
  std::ifstream inputStream(path.generic_string());
  std::string unparsedJson(std::istreambuf_iterator<char>{inputStream}, {});

  return nlohmann::json::parse(unparsedJson)["database"];
}

int main(void) {
  char *roleId = std::getenv("APPROLE_ROLE_ID");
  char *secretId = std::getenv("APPROLE_SECRET_ID");
  if (!roleId && !secretId) {
    std::cout << "APPROLE_ROLE_ID and APPROLE_SECRET_ID environment variables must be set" << std::endl;
    exit(-1);
  }
  Vault::AppRoleStrategy appRoleStrategy{Vault::RoleId{roleId}, Vault::SecretId{secretId}};
  Vault::Config config = Vault::ConfigBuilder().withDebug(false).withTlsEnabled(false).build();
  Vault::Client client = Vault::Client{config, appRoleStrategy};

  if (client.is_authenticated()) {
    std::cout << "Authenticated to Vault" << std::endl;
    Database databaseConfig = getDatabaseConfiguration(std::filesystem::path{"config.json"});
    std::cout << databaseConfig.username << " : " << databaseConfig.password << std::endl;
    Database databaseConfigWithSecrets = databaseConfig.withSecrets(client);
    std::cout << databaseConfigWithSecrets.username << " : " << databaseConfigWithSecrets.password << std::endl;
  } else {
    std::cout << "Unable to authenticate to Vault" << std::endl;
  }
}
