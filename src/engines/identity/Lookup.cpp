#include "VaultClient.h"

std::optional<std::string>
Vault::Identity::Lookup::lookupEntity(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"entity"}), parameters);
}

std::optional<std::string>
Vault::Identity::Lookup::lookupGroup(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"group"}), parameters);
}

Vault::Url Vault::Identity::Lookup::getUrl(const Path &path) {
  return client_.getUrl("/v1/identity/lookup/", path);
}
