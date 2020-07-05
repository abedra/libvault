#include "VaultClient.h"

std::optional<std::string> Vault::Sys::GenerateRoot::read() {
  return HttpConsumer::get(client_, getUrl(Path{"attempt"}));
}

std::optional<std::string> Vault::Sys::GenerateRoot::start(const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"attempt"}), parameters);
}

std::optional<std::string> Vault::Sys::GenerateRoot::update(const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"update"}), parameters);
}

std::optional<std::string> Vault::Sys::GenerateRoot::cancel() {
  return HttpConsumer::del(client_, getUrl(Path{"attempt"}));
}

Vault::Url Vault::Sys::GenerateRoot::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/generate-root/", path);
}
