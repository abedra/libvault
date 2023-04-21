#include "VaultClient.h"

std::optional<std::string>
Vault::GoogleCloud::Auth::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::GoogleCloud::Auth::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string>
Vault::GoogleCloud::Auth::createRole(const Path &path,
                                     const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string>
Vault::GoogleCloud::Auth::editIAMServiceAccount(const Path &path,
                                                const Parameters &parameters) {
  return HttpConsumer::post(
      client_, getUrl(Path{"role/" + path + "/service-accounts"}), parameters);
}

std::optional<std::string>
Vault::GoogleCloud::Auth::editGCERolesLabel(const Path &path,
                                            const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path + "/labels"}),
                            parameters);
}

std::optional<std::string>
Vault::GoogleCloud::Auth::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::GoogleCloud::Auth::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string>
Vault::GoogleCloud::Auth::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string>
Vault::GoogleCloud::Auth::login(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::GoogleCloud::Auth::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/gcp/", path);
}
