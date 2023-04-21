#include "VaultClient.h"

std::optional<std::string>
Vault::UserPass::createUser(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"users/" + path}), parameters);
}

std::optional<std::string>
Vault::UserPass::updateUser(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"users/" + path}), parameters);
}

std::optional<std::string> Vault::UserPass::readUser(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"users/" + path}));
}

std::optional<std::string> Vault::UserPass::deleteUser(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"users/" + path}));
}

std::optional<std::string>
Vault::UserPass::updatePassword(const Path &path,
                                const Parameters &parameters) {
  return HttpConsumer::post(
      client_, getUrl(Path{"users/" + path + "/password"}), parameters);
}

std::optional<std::string>
Vault::UserPass::updatePolicy(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(
      client_, getUrl(Path{"users/" + path + "/policies"}), parameters);
}

std::optional<std::string> Vault::UserPass::listUsers() {
  return HttpConsumer::list(client_, getUrl(Path{"users"}));
}

std::optional<std::string>
Vault::UserPass::login(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login/" + path}), parameters);
}

Vault::Url Vault::UserPass::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/userpass/", path);
}
