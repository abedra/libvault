#include "VaultClient.h"

std::optional<std::string> Vault::GitHub::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::GitHub::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::GitHub::mapGithubTeams(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"map/teams/" + path}), parameters);
}

std::optional<std::string> Vault::GitHub::readTeamMapping(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"map/teams/" + path}));
}

std::optional<std::string> Vault::GitHub::mapGithubUsers(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"map/users/" + path}), parameters);
}

std::optional<std::string> Vault::GitHub::readUserMapping(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"map/users/" + path}));
}

std::optional<std::string> Vault::GitHub::login(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

Vault::Url Vault::GitHub::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/github/", path);
}
