#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Mounts::list() const {
  return HttpConsumer::get(client_, getUrl(Path{"mounts"}));
}

std::optional<std::string> Vault::Sys::Mounts::readUi() const {
  return HttpConsumer::get(client_, getUrl(Path{"internal/ui/mounts"}));
}

std::optional<std::string> Vault::Sys::Mounts::read() const {
  return HttpConsumer::get(client_, getUrl(Path{"mounts"}));
}

std::optional<std::string> Vault::Sys::Mounts::enable(const Path &path,
                                                      const Parameters &parameters,
                                                      const Parameters &options,
                                                      const Parameters &config) const {
  return HttpConsumer::post(client_, getUrl(Path{"mounts/" + path}), parameters, options, config);
}

std::optional<std::string> Vault::Sys::Mounts::disable(const Path &path) const {
  return HttpConsumer::del(client_, getUrl(Path{"mounts/" + path}));
}

std::optional<std::string> Vault::Sys::Mounts::readConfiguration(const Path &path) const {
  return HttpConsumer::get(client_, getUrl(Path{"mounts/" + path + "/tune"}));
}

std::optional<std::string> Vault::Sys::Mounts::tuneConfiguration(const Path &path,
                                                                 const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{"mounts/" + path + "/tune"}), parameters);
}

Vault::Url Vault::Sys::Mounts::getUrl(const Path &path) const {
  return client_.getUrl("/v1/sys/", path);
}
