#include "VaultClient.h"

std::optional<std::string> Vault::AWS::configureRoot(const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"config/root"}), parameters);
}

std::optional<std::string> Vault::AWS::readRoot() {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"config/root"}));
}

std::optional<std::string> Vault::AWS::rotateRoot() {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"config/rotate-root"}), Parameters{});
}

std::optional<std::string> Vault::AWS::configureLease(const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"config/lease"}), parameters);
}

std::optional<std::string> Vault::AWS::readLease() {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"config/lease"}));
}

std::optional<std::string> Vault::AWS::createRole(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::AWS::updateRole(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::AWS::readRole(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::AWS::listRoles() {
  return Vault::HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::AWS::deleteRole(const Path &path) {
  return Vault::HttpConsumer::del(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::AWS::generateCredentials(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"creds/" + path}));
}

std::optional<std::string> Vault::AWS::generateStsCredentials(const Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"sts/" + path}));
}

Vault::Url Vault::AWS::getUrl(const Path &path) {
  return client_.getUrl("/v1/aws/", path);
}
