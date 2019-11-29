#include <utility>
#include <nlohmann/json.hpp>
#include "VaultClient.h"

AppRoleStrategy::AppRoleStrategy(RoleId roleId, SecretId secretId)
  : roleId_(std::move(roleId))
  , secretId_(std::move(secretId))
  {}

optional<AuthenticationResponse> AppRoleStrategy::authenticate(const VaultClient& client) {
  nlohmann::json j;
  j = nlohmann::json::object();
  j["role_id"] = roleId_.value;
  j["secret_id"] = secretId_.value();

  auto response = client.getHttpClient().post(
    getUrl(client, Path{"/login"}),
    client.getToken(),
	  client.getNamespace(),
	  j.dump()
  );

  if (HttpClient::is_success(response)) {
    auto body = HttpResponseBodyString{response.value().body};
    auto token = Token{nlohmann::json::parse(body.value())["auth"]["client_token"]};

    return AuthenticationResponse{body, token};
  } else {
    return std::experimental::nullopt;
  }
}

Url AppRoleStrategy::getUrl(const VaultClient& client, const Path& path) {
  return client.getUrl("/v1/auth/approle", path);
}
