#include "VaultClient.h"

std::optional<std::string>
Vault::AWS::Secrets::configureRoot(const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"config/root"}),
                                   parameters);
}

std::optional<std::string> Vault::AWS::Secrets::readRoot() {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"config/root"}));
}

std::optional<std::string> Vault::AWS::Secrets::rotateRoot() {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"config/rotate-root"}),
                                   Parameters{});
}

std::optional<std::string>
Vault::AWS::Secrets::configureLease(const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"config/lease"}),
                                   parameters);
}

std::optional<std::string> Vault::AWS::Secrets::readLease() {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"config/lease"}));
}

std::optional<std::string>
Vault::AWS::Secrets::createRole(const Path &path,
                                const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}),
                                   parameters);
}

std::optional<std::string>
Vault::AWS::Secrets::updateRole(const Path &path,
                                const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}),
                                   parameters);
}

std::optional<std::string> Vault::AWS::Secrets::readRole(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::AWS::Secrets::listRoles() {
  return Vault::HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::AWS::Secrets::deleteRole(const Path &path) {
  return Vault::HttpConsumer::del(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string>
Vault::AWS::Secrets::generateCredentials(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"creds/" + path}));
}

std::optional<std::string>
Vault::AWS::Secrets::generateStsCredentials(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"sts/" + path}));
}

Vault::Url Vault::AWS::Secrets::getUrl(const Path &path) {
  return client_.getUrl("/v1/aws/", path);
}
