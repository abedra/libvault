#include <VaultClient.h>
#include <nlohmann/json.hpp>

Url Unwrap::getUrl(const VaultClient& client, const Path& path) {
  return client.getUrl("/v1/sys/wrapping", path);
}

optional<SecretId>
Unwrap::unwrap(const VaultClient& client, const Token& token) {
  nlohmann::json j;
  j = nlohmann::json::object();
  j["token"] = token.value;

  auto response = client.getHttpClient().post(
    getUrl(client, Path{"/unwrap"}),
    client.getToken(),
    client.getNamespace(),
    j.dump()
  );

    if (HttpClient::is_success(response)) {
        return SecretId{nlohmann::json::parse(response.value().body.value)["data"]["secret_id"]};
    } else {
        return std::experimental::nullopt;
    }
}
