#include "VaultClient.h"

std::optional<std::string> Vault::Tls::createRole(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"certs/" + path}), parameters);
}

std::optional<std::string> Vault::Tls::readRole(const Path &path) const {
  return HttpConsumer::get(client_, getUrl(Path{"certs/" + path}));
}

std::optional<std::string> Vault::Tls::listRoles() const {
  return HttpConsumer::list(client_, getUrl(Path{"certs"}));
}

std::optional<std::string> Vault::Tls::deleteRole(const Path &path) const {
  return HttpConsumer::del(client_, getUrl(Path{"certs/" + path}));
}

std::optional<std::string> Vault::Tls::createCrl(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"crls/" + path}), parameters);
}

std::optional<std::string> Vault::Tls::readCrl(const Path &path) const {
  return HttpConsumer::get(client_, getUrl(Path{"crls/" + path}));
}

std::optional<std::string> Vault::Tls::deleteCrl(const Path &path) const {
  return HttpConsumer::del(client_, getUrl(Path{"crls/" + path}));
}

std::optional<std::string> Vault::Tls::configure(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Tls::login(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::Tls::getUrl(const Path &path) const {
  return client_.getUrl("/v1/auth/cert/", path);
}
