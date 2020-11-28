#include "VaultClient.h"

std::optional<std::string> Vault::Ldap::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Ldap::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Ldap::listGroups() {
  return HttpConsumer::list(client_, getUrl(Path{"groups"}));
}

std::optional<std::string> Vault::Ldap::readGroup(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"groups/" + path}));
}

std::optional<std::string> Vault::Ldap::createGroup(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"groups/" + path}), parameters);
}

std::optional<std::string> Vault::Ldap::updateGroup(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"groups/" + path}), parameters);
}

std::optional<std::string> Vault::Ldap::deleteGroup(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"groups/" + path}));
}

std::optional<std::string> Vault::Ldap::listUsers() {
  return HttpConsumer::list(client_, getUrl(Path{"users"}));
}

std::optional<std::string> Vault::Ldap::readUser(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"users/" + path}));
}

std::optional<std::string> Vault::Ldap::createUser(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"users/" + path}), parameters);
}

std::optional<std::string> Vault::Ldap::updateUser(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"users/" + path}), parameters);
}

std::optional<std::string> Vault::Ldap::deleteUser(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"users/" + path}));
}

std::optional<std::string> Vault::Ldap::login(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login/" + path}), parameters);
}

Vault::Url Vault::Ldap::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/ldap/", path);
}
