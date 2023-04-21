#include "VaultClient.h"

std::optional<std::string>
Vault::GoogleCloud::Secrets::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::GoogleCloud::Secrets::rotateRoot() {
  return HttpConsumer::post(client_, getUrl(Path{"config/rotate-root"}),
                            Parameters{});
}

std::optional<std::string> Vault::GoogleCloud::Secrets::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string>
Vault::GoogleCloud::Secrets::createRoleset(const Path &path,
                                           const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roleset/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::GoogleCloud::Secrets::updateRoleset(const Path &path,
                                           const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roleset/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::GoogleCloud::Secrets::rotateRolesetAccount(const Path &path) {
  return HttpConsumer::post(
      client_, getUrl(Path{"roleset/" + path + "/rotate"}), Parameters{});
}

std::optional<std::string>
Vault::GoogleCloud::Secrets::rotateRolesetAccountKey(const Path &path) {
  return HttpConsumer::post(
      client_, getUrl(Path{"roleset/" + path + "/rotate-key"}), Parameters{});
}

std::optional<std::string>
Vault::GoogleCloud::Secrets::readRoleset(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"roleset/" + path}));
}

std::optional<std::string> Vault::GoogleCloud::Secrets::listRolesets() {
  return HttpConsumer::list(client_, getUrl(Path{"rolesets"}));
}

std::optional<std::string>
Vault::GoogleCloud::Secrets::deleteRoleset(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"roleset/" + path}));
}

std::optional<std::string>
Vault::GoogleCloud::Secrets::generateSecretToken(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"token/" + path}));
}

std::optional<std::string>
Vault::GoogleCloud::Secrets::generateSecretKey(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"key/" + path}));
}

Vault::Url Vault::GoogleCloud::Secrets::getUrl(const Path &path) {
  return client_.getUrl("/v1/gcp/", path);
}
