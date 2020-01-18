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
  return VaultHttpConsumer::post(client_, getUrl(Vault::Path{"/role" + roleName}), parameters, [&](auto params) {
    nlohmann::json j;
    j["data"] = nlohmann::json::object();
    std::for_each(
      parameters.begin(),
      parameters.end(),
      [&](std::pair<std::string, std::string> pair) {
        j["data"][pair.first] = pair.second;
      }
    );
    return j;
  });
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
