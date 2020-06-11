#include "VaultClient.h"

std::optional<std::string> Vault::ActiveDirectory::createConfig(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::ActiveDirectory::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::ActiveDirectory::deleteConfig() {
  return HttpConsumer::del(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::ActiveDirectory::readRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::ActiveDirectory::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::ActiveDirectory::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::ActiveDirectory::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::ActiveDirectory::readCredentials(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"creds/" + path}));
}

std::optional<std::string> Vault::ActiveDirectory::readServiceAccounts() {
  return HttpConsumer::list(client_, getUrl(Path{"library"}));
}

std::optional<std::string> Vault::ActiveDirectory::readServiceAccount(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"library/" + path}));
}

std::optional<std::string> Vault::ActiveDirectory::createServiceAccount(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"library/" + path}), parameters);
}

std::optional<std::string> Vault::ActiveDirectory::deleteServiceAccount(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"library/" + path}));
}

std::optional<std::string> Vault::ActiveDirectory::checkout(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"library/" + path + "/check-out"}), parameters);
}

std::optional<std::string> Vault::ActiveDirectory::checkin(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"library/" + path + "/check-in"}), parameters);
}

std::optional<std::string> Vault::ActiveDirectory::manageCheckin(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"library/manage/" + path + "/check-in"}), parameters);
}

std::optional<std::string> Vault::ActiveDirectory::status(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"library/" + path + "/status"}));
}

std::optional<std::string> Vault::ActiveDirectory::rotateRootCredentials() {
  return HttpConsumer::get(client_, getUrl(Path{"rotate-root"}));
}

Vault::Url Vault::ActiveDirectory::getUrl(const Path &path) {
  return client_.getUrl("/v1/ad/", path);
}
