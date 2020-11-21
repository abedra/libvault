#include "VaultClient.h"

std::optional<std::string> Vault::OpenLDAP::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::OpenLDAP::readConfiguration() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::OpenLDAP::deleteConfiguration() {
  return HttpConsumer::del(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::OpenLDAP::listStaticRoles() {
  return HttpConsumer::get(client_, getUrl(Path{"static-role"}));
}

std::optional<std::string> Vault::OpenLDAP::readStaticRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"static-role/" + path}));
}

std::optional<std::string> Vault::OpenLDAP::createStaticRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"static-role/" + path}), parameters);
}

std::optional<std::string> Vault::OpenLDAP::deleteStaticRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"static-role" + path}));
}

std::optional<std::string> Vault::OpenLDAP::getStaticRolePassword(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"static-cred/" + path}));
}

std::optional<std::string> Vault::OpenLDAP::rotateStaticRolePassword(const Path &path) {
  return HttpConsumer::post(client_, getUrl(Path{"rotate-role/" + path}), Parameters{});
}

std::optional<std::string> Vault::OpenLDAP::rotateRootPassword() {
  return HttpConsumer::post(client_, getUrl(Path{"rotate-root"}), Parameters{});
}

Vault::Url Vault::OpenLDAP::getUrl(const Path &path) {
  return client_.getUrl("/v1/openldap/", path);
}
