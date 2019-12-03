#include "json.hpp"
#include "VaultClient.h"

Url Unwrap::getUrl(const VaultClient& client, const Path& path) {
  return client.getUrl("/v1/sys/wrapping", path);
}

optional<SecretId>
Unwrap::unwrap(const VaultClient& client) {
  auto response = client.getHttpClient().post(
    getUrl(client, Path{"/unwrap"}),
    client.getToken(),
    client.getNamespace(),
    ""
  );

    if (HttpClient::is_success(response)) {
      return SecretId{nlohmann::json::parse(response.value().body.value())["data"]["secret_id"]};
    } else {
      return std::nullopt;
    }
}
