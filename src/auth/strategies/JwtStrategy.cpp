#include "VaultClient.h"
#include "json.hpp"

std::optional<Vault::AuthenticationResponse>
Vault::JwtStrategy::authenticate(const Vault::Client &client) {
  return HttpConsumer::authenticate(client, getUrl(client, Vault::Path{"/login"}), [this]() {
    nlohmann::json j;
    j = nlohmann::json::object();
    j["role"] = role_.value();
    j["jwt"] = jwt_.value();
    return j.dump();
  });
}

Vault::Url Vault::JwtStrategy::getUrl(const Vault::Client &client,
                                      const Vault::Path &path) {
  return client.getUrl("/v1/auth/" + mount_, path);
}
