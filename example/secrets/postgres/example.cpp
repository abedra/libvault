#include "../../../lib/json.hpp"
#include "../../shared/shared.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <pqxx/pqxx>

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
      std::unordered_map<std::string, std::string> secrets =
          nlohmann::json::parse(databaseSecrets.value())["data"]["data"];
      auto maybeUsername = secrets.find(this->username);
      auto maybePassword = secrets.find(this->password);
      this->username = maybeUsername == secrets.end() ? this->username
                                                      : maybeUsername->second;
      this->password = maybePassword == secrets.end() ? this->password
                                                      : maybePassword->second;

      return *this;
    } else {
      return *this;
    }
  }

  std::string connectionString() {
    std::stringstream ss;
    ss << "hostaddr=" << host << " "
       << "port=" << port << " "
       << "user=" << username << " "
       << "password=" << password << " "
       << "dbname=" << name;
    return ss.str();
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

pqxx::connection getConnection(Database &database) {
  return pqxx::connection{database.connectionString()};
}

int main(void) {
  char *rootTokenEnv = std::getenv("VAULT_ROOT_TOKEN");
  if (!rootTokenEnv) {
    std::cout << "The VAULT_ROOT_TOKEN environment variable must be set"
              << std::endl;
    exit(-1);
  }
  Vault::Token rootToken{rootTokenEnv};
  Vault::Client rootClient = getRootClient(rootToken);

  if (rootClient.is_authenticated()) {
    Vault::KeyValue kv{rootClient};
    Vault::Path key{"database"};
    Vault::Parameters parameters(
        {{"vault:dbuser", "postgres"}, {"vault:dbpass", "postgres"}});

    kv.create(key, parameters);
    Database databaseConfig =
        getDatabaseConfiguration(std::filesystem::path{"config.json"});
    std::cout << "From Config" << std::endl;
    std::cout << databaseConfig.username << " : " << databaseConfig.password
              << std::endl
              << std::endl;

    Database databaseConfigWithSecrets = databaseConfig.withSecrets(rootClient);
    std::cout << "With Secrets" << std::endl;
    std::cout << databaseConfigWithSecrets.username << " : "
              << databaseConfigWithSecrets.password << std::endl
              << std::endl;

    auto conn = getConnection(databaseConfigWithSecrets);
    if (conn.is_open()) {
      std::cout << "Connected to database" << std::endl;
      conn.close();
    } else {
      std::cout << "Could not connect to database" << std::endl;
    }
  } else {
    std::cout << "Unable to authenticate to Vault" << std::endl;
  }
}
