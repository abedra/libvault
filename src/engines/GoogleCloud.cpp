#include "VaultClient.h"

std::optional<std::string> Vault::GoogleCloud::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::GoogleCloud::rotateRoot() {
  return HttpConsumer::post(client_, getUrl(Path{"config/rotate-root"}), Parameters{});
}

std::optional<std::string> Vault::GoogleCloud::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::GoogleCloud::createRoleset(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roleset/" + path}), parameters);
}

std::optional<std::string> Vault::GoogleCloud::updateRoleset(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roleset/" + path}), parameters);
}

std::optional<std::string> Vault::GoogleCloud::rotateRolesetAccount(const Path &path) {
  return HttpConsumer::post(client_, getUrl(Path{"roleset/" + path + "/rotate"}), Parameters{});
}

std::optional<std::string> Vault::GoogleCloud::rotateRolesetAccountKey(const Path &path) {
  return HttpConsumer::post(client_, getUrl(Path{"roleset/" + path + "/rotate-key"}), Parameters{});
}

std::optional<std::string> Vault::GoogleCloud::readRoleset(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"roleset/" + path}));
}

std::optional<std::string> Vault::GoogleCloud::listRolesets() {
  return HttpConsumer::list(client_, getUrl(Path{"rolesets"}));
}

std::optional<std::string> Vault::GoogleCloud::deleteRoleset(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"roleset/" + path}));
}

std::optional<std::string> Vault::GoogleCloud::generateSecretToken(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"token/" + path}));
}

std::optional<std::string> Vault::GoogleCloud::generateSecretKey(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"key/" + path}));
}

Vault::Url Vault::GoogleCloud::getUrl(const Path &path) {
  return client_.getUrl("/v1/gcp/", path);
}
