#include <utility>

#include "json.hpp"
#include "VaultClient.h"

LdapStrategy::LdapStrategy(std::string  username, std::string  password)
  : username_(std::move(username))
  , password_(std::move(password))
  {}

std::optional<AuthenticationResponse> LdapStrategy::authenticate(const VaultClient &client) {
  return VaultHttpConsumer::authenticate(
    client,
    getUrl(client, Vault::Path{username_}),
    [&]() {
      nlohmann::json j;
      j = nlohmann::json::object();
      j["password"] = password_;
      return j;
    }
  );
}

Vault::Url LdapStrategy::getUrl(const VaultClient& client, const Vault::Path& username) {
  return client.getUrl("/v1/auth/ldap/login/", username);
}
