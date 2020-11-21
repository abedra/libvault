#include "VaultClient.h"

std::optional<std::string> Vault::Nomad::configureAccess(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config/access"}), parameters);
}

std::optional<std::string> Vault::Nomad::readAccessConfiguration() {
  return HttpConsumer::get(client_, getUrl(Path{"config/access"}));
}

std::optional<std::string> Vault::Nomad::configureLease(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config/least"}), parameters);
}

std::optional<std::string> Vault::Nomad::readLeaseConfiguration() {
  return HttpConsumer::get(client_, getUrl(Path{"config/lease"}));
}

std::optional<std::string> Vault::Nomad::deleteLeaseConfiguration() {
  return HttpConsumer::del(client_, getUrl(Path{"config/lease"}));
}

std::optional<std::string> Vault::Nomad::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::Nomad::updateRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::Nomad::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::Nomad::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"role"}));
}

std::optional<std::string> Vault::Nomad::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::Nomad::generateCredential(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"creds/" + path}));
}

Vault::Url Vault::Nomad::getUrl(const Path &path) {
  return client_.getUrl("/v1/nomad/", path);
}
