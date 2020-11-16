#include "VaultClient.h"

std::optional<std::string> Vault::Kmip::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Kmip::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Kmip::readCa() {
  return HttpConsumer::get(client_, getUrl(Path{"ca"}));
}

std::optional<std::string> Vault::Kmip::writeScope(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_,getUrl(Path{"scope/" + path}), parameters);
}

std::optional<std::string> Vault::Kmip::listScopes() {
  return HttpConsumer::list(client_, getUrl(Path{"scope"}));
}

std::optional<std::string> Vault::Kmip::deleteScope(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"scope/" + path}));
}

std::optional<std::string> Vault::Kmip::writeRole(const Path &scope, const Path &role, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"scope/" + scope + "/role/" + role}), parameters);
}

std::optional<std::string> Vault::Kmip::readRole(const Path &scope, const Path &role) {
  return HttpConsumer::get(client_,getUrl(Path{"scope/" + scope+ "/role/" + role}));
}

std::optional<std::string> Vault::Kmip::listRoles(const Path &path) {
  return HttpConsumer::list(client_, getUrl(Path{"scope/" + path + "/role"}));
}

std::optional<std::string> Vault::Kmip::deleteRole(const Path &scope, const Path &role) {
  return HttpConsumer::del(client_, getUrl(Path{"scope/" + scope + "/role/" + role}));
}

std::optional<std::string> Vault::Kmip::generateCredential(const Path &scope, const Path &role,
                                                           const Parameters &parameters) {
  return HttpConsumer::post(
      client_,
      getUrl(Path{"scope/" + scope + "/role/" + role + "/credential/generate"}),
      parameters
  );
}

std::optional<std::string> Vault::Kmip::signCsr(const Path &scope, const Path &role, const Parameters &parameters) {
  return HttpConsumer::post(
      client_,
      getUrl(Path{"scope/" + scope + "/role/" + role + "credential/sign"}),
      parameters
  );
}

std::optional<std::string> Vault::Kmip::lookupCredential(const Path &scope, const Path &role) {
  return HttpConsumer::get(
      client_,
      getUrl(Path{"scope/" + scope + "/role/" + role + "/credential/lookup"})
  );
}

std::optional<std::string> Vault::Kmip::listCredentialSerialNumbers(const Path &scope, const Path &role) {
  return HttpConsumer::list(client_, getUrl(Path{"scope/" + scope + "/role/" + role + "/credential"}));
}

std::optional<std::string> Vault::Kmip::revokeCredential(const Path &scope, const Path &role,
                                                         const Parameters &parameters) {
  return HttpConsumer::post(
      client_,
      getUrl(Path{"scope/" + scope + "/role/" + role + "credential/revoke"}),
      parameters
  );
}

Vault::Url Vault::Kmip::getUrl(const Path &path) {
  return client_.getUrl("/v1/kmip/", path);
}
