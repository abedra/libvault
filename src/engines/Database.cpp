#include "VaultClient.h"

std::optional<std::string> Vault::Database::configureConnection(const Path &path, const Parameters &parameters) {
    return HttpConsumer::post(client_, getUrl(Vault::Path{"config/" + path}), parameters);
}

std::optional<std::string> Vault::Database::readConnection(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"config/" + path}));
}

std::optional<std::string> Vault::Database::listConnections() {
  return HttpConsumer::list(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::Database::deleteConnection(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"config/" + path}));
}

std::optional<std::string> Vault::Database::resetConnection(const Path &path) {
  return HttpConsumer::post(client_, getUrl(Path{"reset/" + path}), Parameters{});
}

std::optional<std::string> Vault::Database::rotateRootCredentials(const Path &path) {
  return HttpConsumer::post(client_, getUrl(Path{"rotate-root/" + path}), Parameters{});
}

std::optional<std::string> Vault::Database::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Vault::Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::Database::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Vault::Database::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::Database::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Vault::Path{"roles/" + path}));
}

std::optional<std::string> Vault::Database::generateCredentials(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Vault::Path{"creds/" + path}));
}

std::optional<std::string> Vault::Database::createStaticRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"static-roles/" + path}), parameters);
}

std::optional<std::string> Vault::Database::readStaticRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"static-roles/" + path}));
}

std::optional<std::string> Vault::Database::listStaticRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"static-roles"}));
}

std::optional<std::string> Vault::Database::deleteStaticRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"static-roles/" + path}));
}

std::optional<std::string> Vault::Database::getStaticCredentials(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"static-creds/" + path}));
}

std::optional<std::string> Vault::Database::rotateStaticCredentials(const Path &path) {
  return HttpConsumer::post(client_, getUrl(Path{"rotate-role/" + path}), Parameters{});
}

Vault::Url Vault::Database::getUrl(const Path &path) {
  return client_.getUrl("/v1/database/", path);
}
