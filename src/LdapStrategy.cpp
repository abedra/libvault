#include <utility>

#include "json.hpp"
#include "VaultClient.h"

Vault::LdapStrategy::LdapStrategy(std::string  username, std::string  password)
  : username_(std::move(username))
  , password_(std::move(password))
  {}

std::optional<Vault::AuthenticationResponse>
Vault::LdapStrategy::authenticate(const Vault::Client &client) {
  return Vault::HttpConsumer::authenticate(
    client,
    getUrl(client, Vault::Path{username_}),
    [&]() {
      nlohmann::json j;
      j = nlohmann::json::object();
      j["password"] = password_;
      return j.dump();
    }
  );
}

Vault::Url Vault::LdapStrategy::getUrl(const Vault::Client& client, const Vault::Path& username) {
  return client.getUrl("/v1/auth/ldap/login/", username);
}
