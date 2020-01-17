#include <utility>
#include "json.hpp"
#include "VaultClient.h"

AppRole::AppRole(const VaultClient& vaultClient)
  : client_(vaultClient)
  {}

std::optional<std::string> AppRole::list() {
  return VaultHttpConsumer::list(client_, getUrl(Vault::Path{"/role"}));
}

std::optional<std::string> AppRole::create(const Vault::Path& roleName, const Parameters& parameters) {
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

  Vault::Path path{"/role"};
  auto response = client_.getHttpClient().post(
    getUrl(Vault::Path{path + roleName}),
    client_.getToken(),
    client_.getNamespace(),
    j.dump()
  );

  return response
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string> AppRole::update(const Vault::Path& roleName, const Parameters& parameters) {
  return create(roleName, parameters);
}

std::optional<std::string> AppRole::read(const Vault::Path& roleName) {
  return VaultHttpConsumer::get(client_, getUrl(Vault::Path{"/role" + roleName}));
}

Vault::Url AppRole::getUrl(const Vault::Path& path) {
  return client_.getUrl("/v1/auth/approle", path);
}
