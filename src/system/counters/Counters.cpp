#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Counters::entities() {
  return HttpConsumer::get(client_, getUrl(Path{"entities"}));
}

std::optional<std::string> Vault::Sys::Counters::requests() {
  return HttpConsumer::get(client_, getUrl(Path{"requests"}));
}

std::optional<std::string> Vault::Sys::Counters::tokens() {
  return HttpConsumer::get(client_, getUrl(Path{"tokens"}));
}

Vault::Url Vault::Sys::Counters::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/internal/counters/", path);
}
