#include "VaultClient.h"

std::optional<std::string> Vault::JwtOidc::configure(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::JwtOidc::readConfig() const {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::JwtOidc::createRole(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::JwtOidc::readRole(const Path &path) const {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::JwtOidc::listRoles() const {
  return HttpConsumer::list(client_, getUrl(Path{"role"}));
}

std::optional<std::string> Vault::JwtOidc::deleteRole(const Path &path) const {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::JwtOidc::oidcAuthorizationUrlRequest(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"oidc/auth_url"}), parameters);
}

std::optional<std::string> Vault::JwtOidc::oidcCallback() const {
  return HttpConsumer::get(client_, getUrl(Path{"oidc/callback"}));
}

std::optional<std::string> Vault::JwtOidc::jwtLogin(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::JwtOidc::getUrl(const Path &path) const {
  return client_.getUrl("/v1/auth/jwt/", path);
}
