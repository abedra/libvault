#include "VaultClient.h"

std::optional<std::string> Vault::Cubbyhole::create(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(path),
    parameters
  );
}

std::optional<std::string> Vault::Cubbyhole::read(const Path &path) {
  return Vault::HttpConsumer::get(
    client_,
    getUrl(path)
  );
}

std::optional<std::string> Vault::Cubbyhole::list(const Path &path) {
  return Vault::HttpConsumer::list(
    client_,
    getUrl(path)
  );
}

std::optional<std::string> Vault::Cubbyhole::del(const Path &path) {
  return Vault::HttpConsumer::del(
    client_,
    getUrl(path)
  );
}

Vault::Url Vault::Cubbyhole::getUrl(const Path &path) {
  return client_.getUrl("/v1/cubbyhole/", path);
}

