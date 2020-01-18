#include "json.hpp"
#include "VaultClient.h"

Vault::Url Unwrap::getUrl(const VaultClient& client, const Vault::Path& path) {
  return client.getUrl("/v1/sys/wrapping", path);
}

std::optional<Vault::SecretId>
Unwrap::unwrap(const VaultClient& client) {
  auto response = client.getHttpClient().post(
    getUrl(client, Vault::Path{"/unwrap"}),
    client.getToken(),
    client.getNamespace(),
    ""
  );

  if (HttpClient::is_success(response)) {
    return Vault::SecretId{nlohmann::json::parse(response.value().body.value())["data"]["secret_id"]};
  } else {
    return std::nullopt;
  }
}
