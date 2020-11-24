#include "VaultClient.h"

std::optional<std::string> Vault::Okta::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Okta::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Okta::listUsers() {
  return HttpConsumer::list(client_, getUrl(Path{"users"}));
}

std::optional<std::string> Vault::Okta::registerUser(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"users/" + path}), parameters);
}

std::optional<std::string> Vault::Okta::readUser(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"users/" + path}));
}

std::optional<std::string> Vault::Okta::deleteUser(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"users/" + path}));
}

std::optional<std::string> Vault::Okta::listGroups() {
  return HttpConsumer::list(client_, getUrl(Path{"groups"}));
}

std::optional<std::string> Vault::Okta::registerGroup(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"groups/" +path}), parameters);
}

std::optional<std::string> Vault::Okta::readGroup(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"groups/" + path}));
}

std::optional<std::string> Vault::Okta::deleteGroup(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"groups/" + path}));
}

std::optional<std::string> Vault::Okta::login(const Path  &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login" + path}), parameters);
}

Vault::Url Vault::Okta::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/okta/", path);
}

