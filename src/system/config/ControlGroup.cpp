#include "VaultClient.h"

std::optional<std::string> Vault::Sys::ControlGroup::read() {
  return HttpConsumer::get(client_, getUrl(Path{"config/control-group"}));
}

std::optional<std::string>
Vault::Sys::ControlGroup::configure(const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"config/control-group"}),
                           parameters);
}

std::optional<std::string> Vault::Sys::ControlGroup::del() {
  return HttpConsumer::del(client_, getUrl(Path{"config/control-group"}));
}

std::optional<std::string>
Vault::Sys::ControlGroup::authorize(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"control-group/authorize"}),
                            parameters);
}

std::optional<std::string>
Vault::Sys::ControlGroup::request(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"control-group/request"}),
                            parameters);
}

Vault::Url Vault::Sys::ControlGroup::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/", path);
}
