#include <utility>
#include "json.hpp"
#include "VaultClient.h"

AppRole::AppRole(const VaultClient& vaultClient)
  : client_(vaultClient)
  {}

std::optional<std::string> AppRole::list() {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client_.getHttpClient().list(
    getUrl(Path{"/role"}),
    client_.getToken(),
    client_.getNamespace()
  );

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string> AppRole::create(const Path& roleName, const Parameters& parameters) {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  nlohmann::json j;
  j["data"] = nlohmann::json::object();
  std::for_each(
    parameters.begin(),
    parameters.end(),
    [&](std::pair<std::string, std::string> pair) {
      j["data"][pair.first] = pair.second;
    }
  );

  Path path{"/role"};
  auto response = client_.getHttpClient().post(
    getUrl(Path{path + roleName}),
    client_.getToken(),
    client_.getNamespace(),
    j.dump()
  );

  return response
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string> AppRole::update(const Path& roleName, const Parameters& parameters) {
  return create(roleName, parameters);
}

std::optional<std::string> AppRole::read(const Path& roleName) {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  Path path = Path{"/role"};
  auto response = client_.getHttpClient().list(
    getUrl(Path{path + roleName}),
    client_.getToken(),
    client_.getNamespace()
  );

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

Url AppRole::getUrl(const Path& path) {
  return client_.getUrl("/v1/auth/approle", path);
}

