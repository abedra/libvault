#include "VaultClient.h"

std::optional<std::string> Vault::SSH::createKey(const Path &path,
                                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/" + path}), parameters);
}

std::optional<std::string> Vault::SSH::updateKey(const Path &path,
                                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/" + path}), parameters);
}

std::optional<std::string> Vault::SSH::deleteKey(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"keys/" + path}));
}

std::optional<std::string>
Vault::SSH::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::SSH::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::SSH::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::SSH::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::SSH::listZeroAddressRoles() {
  return HttpConsumer::get(client_, getUrl(Path{"config/zeroaddress"}));
}

std::optional<std::string>
Vault::SSH::configureZeroAddressRoles(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config/zeroaddress"}),
                            parameters);
}

std::optional<std::string> Vault::SSH::deleteZeroAddressRole() {
  return HttpConsumer::del(client_, getUrl(Path{"config/zeroaddress"}));
}

std::optional<std::string>
Vault::SSH::generateCredentials(const Path &path,
                                const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"creds/" + path}), parameters);
}

std::optional<std::string>
Vault::SSH::listRolesByIp(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"lookup"}), parameters);
}

std::optional<std::string> Vault::SSH::verifyOtp(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"verify"}), parameters);
}

std::optional<std::string>
Vault::SSH::configureCA(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config/ca"}), parameters);
}

std::optional<std::string> Vault::SSH::deleteCA() {
  return HttpConsumer::del(client_, getUrl(Path{"config/ca"}));
}

std::optional<std::string> Vault::SSH::readPublicKey() {
  return HttpConsumer::get(client_, getUrl(Path{"public_key"}));
}

std::optional<std::string> Vault::SSH::signKey(const Path &path,
                                               const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"sign/" + path}), parameters);
}

Vault::Url Vault::SSH::getUrl(const Path &path) {
  return client_.getUrl("/v1/ssh", path);
}
