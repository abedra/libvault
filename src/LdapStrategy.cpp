#include <utility>

#include "json.hpp"
#include "VaultClient.h"

LdapStrategy::LdapStrategy(std::string  username, std::string  password)
  : username_(std::move(username))
  , password_(std::move(password))
  {}

std::optional<AuthenticationResponse> LdapStrategy::authenticate(const VaultClient &vaultClient) {
  nlohmann::json j;
  j = nlohmann::json::object();
  j["password"] = password_;

  auto response = vaultClient.getHttpClient().post(
    getUrl(vaultClient, Path{username_}),
    vaultClient.getToken(),
    vaultClient.getNamespace(),
    j.dump()
  );

  if (HttpClient::is_success(response)) {
    auto body = HttpResponseBodyString{response.value().body};
    auto token = Token{nlohmann::json::parse(body.value())["auth"]["client_token"]};

    return AuthenticationResponse{body, token};
  } else {
    return std::nullopt;
  }
}

Url LdapStrategy::getUrl(const VaultClient& client, const Path& username) {
  return client.getUrl("/v1/auth/ldap/login/", username);
}
