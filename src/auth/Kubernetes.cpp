#include "VaultClient.h"

std::optional<std::string> Vault::Kubernetes::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::Kubernetes::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Kubernetes::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::Kubernetes::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::Kubernetes::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"role"}));
}

std::optional<std::string> Vault::Kubernetes::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::Kubernetes::login(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::Kubernetes::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/kubernetes/", path);
}
