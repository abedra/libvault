#include "VaultClient.h"

std::optional<std::string> Vault::Totp::create(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"keys/" + path}),
    parameters
  );
}

std::optional<std::string> Vault::Totp::list() {
  return Vault::HttpConsumer::list(
    client_,
    getUrl(Vault::Path{"keys"})
  );
}

std::optional<std::string> Vault::Totp::read(const Path &path) {
  return Vault::HttpConsumer::get(
    client_,
    getUrl(Vault::Path{"keys/" + path})
  );
}

std::optional<std::string> Vault::Totp::generate(const Path &path) {
  return Vault::HttpConsumer::get(
    client_,
    getUrl(Vault::Path{"code/" + path})
  );
}

std::optional<std::string> Vault::Totp::validate(const Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"code/" + path}),
    parameters
  );
}

std::optional<std::string> Vault::Totp::del(const Path &path) {
  return Vault::HttpConsumer::del(
    client_,
    getUrl(Vault::Path{"keys/" + path})
  );
}

Vault::Url Vault::Totp::getUrl(const Path &path) {
  return client_.getUrl("/v1/totp/", path);
}