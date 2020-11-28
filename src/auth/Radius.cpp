#include "VaultClient.h"

std::optional<std::string> Vault::Radius::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Radius::registerUser(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"users/" + path}), parameters);
}

std::optional<std::string> Vault::Radius::readUser(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"users/" + path}));
}

std::optional<std::string> Vault::Radius::deleteUser(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"users/" + path}));
}

std::optional<std::string> Vault::Radius::listUsers() {
  return HttpConsumer::list(client_, getUrl(Path{"users"}));
}

std::optional<std::string> Vault::Radius::login(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::Radius::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/radius/", path);
}
