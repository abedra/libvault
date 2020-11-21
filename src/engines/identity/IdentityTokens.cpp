#include "VaultClient.h"

std::optional<std::string> Vault::Identity::IdentityTokens::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Identity::IdentityTokens::readConfigurations() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Identity::IdentityTokens::createNamedKey(const Path &path,
                                                                           const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"key/" + path}), parameters);
}

std::optional<std::string> Vault::Identity::IdentityTokens::readNamedKey(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"key/" + path}));
}

std::optional<std::string> Vault::Identity::IdentityTokens::deleteNamedKey(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"key/" + path}));
}

std::optional<std::string> Vault::Identity::IdentityTokens::listNamedKeys() {
  return HttpConsumer::del(client_, getUrl(Path{"key"}));
}

std::optional<std::string> Vault::Identity::IdentityTokens::rotateNamedKey(const Path &path,
                                                                           const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"key/" + path + "/rotate"}), parameters);
}

std::optional<std::string> Vault::Identity::IdentityTokens::createRole(const Path &path,
                                                                       const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::Identity::IdentityTokens::updateRole(const Path &path,
                                                                       const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::Identity::IdentityTokens::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::Identity::IdentityTokens::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::Identity::IdentityTokens::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"role"}));
}

std::optional<std::string> Vault::Identity::IdentityTokens::generateSignedIdToken(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"token/" + path}));
}

std::optional<std::string> Vault::Identity::IdentityTokens::introspectSignedIdToken(const Path &path,
                                                                                    const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"introspect"}), parameters);
}

std::optional<std::string> Vault::Identity::IdentityTokens::readWellKnownConfigurations() {
  return HttpConsumer::get(client_, getUrl(Path{".well-known/openid-configuration"}));
}

std::optional<std::string> Vault::Identity::IdentityTokens::readActivePublicKeys() {
  return HttpConsumer::get(client_, getUrl(Path{".well-known/keys"}));
}

Vault::Url Vault::Identity::IdentityTokens::getUrl(const Path &path) {
  return client_.getUrl("/v1/identity/oidc/", path);
}