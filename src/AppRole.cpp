#include <utility>
#include <nlohmann/json.hpp>
#include "VaultClient.h"

AppRole::AppRole(std::string role_id, std::string secret_id)
  : role_id_(std::move(role_id))
  , secret_id_(std::move(secret_id))
  {}

optional<AuthenticationResponse> AppRole::authenticate(const VaultClient& client) {
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
    std::string &body = response.value().body;
    return AuthenticationResponse{body, nlohmann::json::parse(body)["auth"]["client_token"]};
  } else {
    return std::experimental::nullopt;
  }
}

std::string AppRole::getUrl(const VaultClient& client, const std::string& path) {
  return client.getUrl("/v1/auth/approle", path);
}
