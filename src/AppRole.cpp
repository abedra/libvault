#include <nlohmann/json.hpp>
#include "VaultClient.h"

AppRole::AppRole(std::string role_id, std::string secret_id) :
  role_id_(role_id), secret_id_(secret_id) {}

std::string AppRole::getUrl(const VaultClient& client, std::string path) {
  return client.getUrl("/v1/auth/approle", path);
}

optional<std::string> AppRole::authenticate(const VaultClient& client) {
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
    return nlohmann::json::parse(response.value().body)["auth"]["client_token"];
  } else {
    return std::experimental::nullopt;
  }
}
