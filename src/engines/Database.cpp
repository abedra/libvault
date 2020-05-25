#include "VaultClient.h"

std::optional<std::string> Vault::Database::configureConnection(const Path &path, const Parameters &parameters) {
    return HttpConsumer::post(client_, getUrl(Vault::Path{"config/" + path}), parameters);
}

std::optional<std::string> Vault::Database::createRole(const Path &path, const Parameters &parameters) {
    return HttpConsumer::post(client_, getUrl(Vault::Path{"roles/" + path}), parameters);
}

std::optional<std::string> Vault::Database::deleteRole(const Path &path) {
    return HttpConsumer::del(client_, getUrl(Vault::Path{"roles/" + path}));
}

std::optional<std::string> Vault::Database::generateCredentials(const Path &path) {
    return HttpConsumer::get(client_, getUrl(Vault::Path{"creds/" + path}));
}

Vault::Url Vault::Database::getUrl(const Path &path) {
    return client_.getUrl("/v1/database/", path);
}
