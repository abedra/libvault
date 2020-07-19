#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Tools::random(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"random/" + path}), parameters);
}

std::optional<std::string> Vault::Sys::Tools::hash(const Algorithm &algorithm, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"hash/" + algorithm}), parameters);
}

Vault::Url Vault::Sys::Tools::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/tools/", path);
}
