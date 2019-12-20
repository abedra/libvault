#include <utility>
#include "json.hpp"
#include "VaultClient.h"

AppRoleStrategy::AppRoleStrategy(Vault::RoleId roleId, Vault::SecretId secretId)
  : roleId_(std::move(roleId))
  , secretId_(std::move(secretId))
  {}

std::optional<AuthenticationResponse> AppRoleStrategy::authenticate(const VaultClient& client) {
  nlohmann::json j;
  j = nlohmann::json::object();
  j["role_id"] = roleId_.value();
  j["secret_id"] = secretId_.value();

  auto response = client.getHttpClient().post(
    getUrl(client, Vault::Path{"/login"}),
    client.getToken(),
	  client.getNamespace(),
	  j.dump()
  );

  if (HttpClient::is_success(response)) {
    auto body = Vault::HttpResponseBodyString{response.value().body};
    auto token = Vault::Token{nlohmann::json::parse(body.value())["auth"]["client_token"]};

    return AuthenticationResponse{body, token};
  } else {
    return std::nullopt;
  }
}

Vault::Url AppRoleStrategy::getUrl(const VaultClient& client, const Vault::Path& path) {
  return client.getUrl("/v1/auth/approle", path);
}
