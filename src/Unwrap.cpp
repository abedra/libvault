#include <VaultClient.h>
#include <nlohmann/json.hpp>

std::string Unwrap::getUrl(const VaultClient& client, const std::string& path) {
  return client.getUrl("/v1/sys/wrapping", path);
}

optional<std::string>
Unwrap::unwrap(const VaultClient& client, std::string token) {
  nlohmann::json j;
  j = nlohmann::json::object();
  j["token"] = token;

  auto response = client
          .getHttpClient()
          .post(getUrl(client, "/unwrap"),
                client.getToken(),
                client.getNamespace(),
                j.dump()
          );

    if (HttpClient::is_success(response)) {
        return nlohmann::json::parse(response.value().body)["data"]["secret_id"];
    } else {
        return std::experimental::nullopt;
    }
}
