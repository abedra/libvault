#include "VaultClient.h"

std::optional<std::string> Vault::Pki::readCACertificate() const {
  return HttpConsumer::get(client_, getUrl(Path{"ca/pem"}));
}

std::optional<std::string> Vault::Pki::readCACertificateChain() const {
  return HttpConsumer::get(client_, getUrl(Path{"ca_chain"}));
}

std::optional<std::string>
Vault::Pki::generateRoot(const RootCertificateType &rootCertificateType,
                         const Parameters &parameters) const {
  return HttpConsumer::post(
      client_, getUrl(Path{"root/generate/" + rootCertificateType}),
      parameters);
}

std::optional<std::string> Vault::Pki::deleteRoot() const {
  return HttpConsumer::del(client_, getUrl(Path{"root"}));
}

std::optional<std::string>
Vault::Pki::setUrls(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"config/urls"}), parameters);
}

std::optional<std::string>
Vault::Pki::createRole(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string>
Vault::Pki::updateRole(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::Pki::readRole(const Path &path) const {
  return HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::Pki::listRoles() const {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::Pki::deleteRole(const Path &path) const {
  return HttpConsumer::del(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string>
Vault::Pki::issue(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"issue/" + path}), parameters);
}

std::optional<std::string> Vault::Pki::readCertificate(const Path &path) const {
  return HttpConsumer::get(client_, getUrl(Path{"cert/" + path}));
}

std::optional<std::string> Vault::Pki::listCertificates() const {
  return HttpConsumer::list(client_, getUrl(Path{"certs"}));
}

std::optional<std::string>
Vault::Pki::configureCA(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"config/ca"}), parameters);
}

std::optional<std::string> Vault::Pki::readCrlConfiguration() const {
  return HttpConsumer::get(client_, getUrl(Path{"config/crl"}));
}

std::optional<std::string>
Vault::Pki::setCrlConfiguration(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"config/crl"}), parameters);
}

std::optional<std::string> Vault::Pki::readURLs() const {
  return HttpConsumer::get(client_, getUrl(Path{"config/urls"}));
}

std::optional<std::string> Vault::Pki::readCRL() const {
  return HttpConsumer::get(client_, getUrl(Path{"crl/pem"}));
}

std::optional<std::string>
Vault::Pki::generateIntermediate(const KeyType &keyType,
                                 const Parameters &parameters) const {
  return HttpConsumer::post(
      client_, getUrl(Path{"intermediate/generate/" + keyType}), parameters);
}

std::optional<std::string>
Vault::Pki::signIntermediate(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"root/sign-intermediate"}),
                            parameters);
}

std::optional<std::string>
Vault::Pki::setSignedIntermediate(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"intermediate/set-signed"}),
                            parameters);
}

std::optional<std::string>
Vault::Pki::signSelfIssued(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"root/sign-self-issued"}),
                            parameters);
}

std::optional<std::string>
Vault::Pki::generateCertificate(const Path &path,
                                const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"issue/" + path}), parameters);
}

std::optional<std::string>
Vault::Pki::sign(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"sign/" + path}), parameters);
}

std::optional<std::string>
Vault::Pki::signVerbatim(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"sign-verbatim/" + path}),
                            parameters);
}

std::optional<std::string> Vault::Pki::rotateCrl() const {
  return HttpConsumer::get(client_, getUrl(Path{"crl/rotate"}));
}

std::optional<std::string>
Vault::Pki::tidy(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"tidy"}), parameters);
}

std::optional<std::string>
Vault::Pki::revokeCertificate(const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"revoke"}), parameters);
}

std::optional<std::string> Vault::Pki::listRevoked() const {
  return HttpConsumer::list(client_, getUrl(Path{"certs/revoked"}));
}

Vault::Url Vault::Pki::getUrl(const Path &path) const {
  return client_.getUrl("/v1/" + secretMount_,
                        path.empty() ? path : Path{"/" + path});
}
