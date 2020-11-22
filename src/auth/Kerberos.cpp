#include "VaultClient.h"

std::optional<std::string> Vault::Kerberos::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Kerberos::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Kerberos::configureLdap(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config/ldap"}), parameters);
}

std::optional<std::string> Vault::Kerberos::readLdapConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config/ldap"}));
}

std::optional<std::string> Vault::Kerberos::listGroups() {
  return HttpConsumer::list(client_, getUrl(Path{"groups"}));
}

std::optional<std::string> Vault::Kerberos::readGroup(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"groups/" + path}));
}

std::optional<std::string> Vault::Kerberos::createGroup(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"groups/" + path}), parameters);
}

std::optional<std::string> Vault::Kerberos::updateGroup(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"groups/" + path}), parameters);
}

std::optional<std::string> Vault::Kerberos::deleteGroup(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"groups/" + path}));
}

std::optional<std::string> Vault::Kerberos::login(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::Kerberos::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/kerberos/", path);
}
