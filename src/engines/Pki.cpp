#include "VaultClient.h"

std::optional<std::string> Vault::Pki::generateRoot(const RootCertificateType &rootCertificateType, const Parameters &parameters) {
  return HttpConsumer::post(
      client_,
      getUrl(Vault::Path{"root/generate/" + rootCertificateType}),
      parameters
  );
}

std::optional<std::string> Vault::Pki::setUrls(const Parameters &parameters) {
  return HttpConsumer::post(
      client_,
      getUrl(Vault::Path{"config/urls"}),
      parameters
  );
}

std::optional<std::string> Vault::Pki::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(
      client_,
      getUrl(Vault::Path{"roles/" + path}),
      parameters
  );
}

std::optional<std::string> Vault::Pki::updateRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(
      client_,
      getUrl(Vault::Path{"roles/" + path}),
      parameters
  );
}

std::optional<std::string> Vault::Pki::issue(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(
      client_,
      getUrl(Vault::Path{"issue/" + path}),
      parameters
  );
}

std::optional<std::string> Vault::Pki::readCertificate(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Vault::Path{"cert/" + path}));
}

std::optional<std::string> Vault::Pki::listCertificates() {
  return HttpConsumer::list(client_, getUrl(Vault::Path{"certs"}));
}

std::optional<std::string> Vault::Pki::readCrlConfiguration() {
  return HttpConsumer::get(client_, getUrl(Vault::Path{"config/crl"}));
}

std::optional<std::string> Vault::Pki::setCrlConfiguration(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Vault::Path{"config/crl"}), parameters);
}

Vault::Url Vault::Pki::getUrl(const Path &path) {
  return client_.getUrl("/v1/pki/", path);
}

