#include "VaultClient.h"

std::optional<std::string> Vault::Sys::UI::list() {
  return HttpConsumer::list(client_, getUrl(Path{}));
}

std::optional<std::string> Vault::Sys::UI::read(const Path &path) {
  return HttpConsumer::get(client_, getUrl(path));
}

std::optional<std::string>
Vault::Sys::UI::configure(const Path &path, const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(path), parameters);
}

std::optional<std::string> Vault::Sys::UI::del(const Path &path) {
  return HttpConsumer::del(client_, getUrl(path));
}

Vault::Url Vault::Sys::UI::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/config/ui/headers/", path);
}
