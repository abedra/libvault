#include <utility>
#include <nlohmann/json.hpp>
#include "VaultClient.h"

AppRoleStrategy::AppRoleStrategy(std::string role_id, std::string secret_id)
  : role_id_(std::move(role_id))
  , secret_id_(std::move(secret_id))
  {}

optional<AuthenticationResponse> AppRoleStrategy::authenticate(const VaultClient& client) {
  nlohmann::json j;
  j = nlohmann::json::object();
  j["role_id"] = role_id_;
  j["secret_id"] = secret_id_;

  auto response = client.getHttpClient()
    .post(getUrl(client, "/login"),
	  client.getToken(),
	  client.getNamespace(),
	  j.dump());

  if (HttpClient::is_success(response)) {
    auto body = HttpResponseBodyString{response.value().body};
    auto token = Token{nlohmann::json::parse(body.value)["auth"]["client_token"]};

    return AuthenticationResponse{body, token};
  } else {
    return std::experimental::nullopt;
  }
}

std::string AppRoleStrategy::getUrl(const VaultClient& client, const std::string& path) {
  return client.getUrl("/v1/auth/approle", path);
}
