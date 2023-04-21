#include "VaultClient.h"

std::optional<std::string>
Vault::RabbitMq::configureConnection(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Vault::Path{"config/connection"}),
                            parameters);
}

std::optional<std::string>
Vault::RabbitMq::configureLease(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Vault::Path{"config/lease"}),
                            parameters);
}

std::optional<std::string>
Vault::RabbitMq::createRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Vault::Path{"roles/" + path}),
                            parameters);
}

std::optional<std::string> Vault::RabbitMq::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Vault::Path{"roles/" + path}));
}

std::optional<std::string> Vault::RabbitMq::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Vault::Path{"roles/" + path}));
}

std::optional<std::string>
Vault::RabbitMq::generateCredentials(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Vault::Path{"creds/" + path}));
}

Vault::Url Vault::RabbitMq::getUrl(const Path &path) {
  return client_.getUrl("/v1/rabbitmq/", path);
}
