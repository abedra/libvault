#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Auth::list() const {
  return HttpConsumer::get(client_, getUrl(Path{}));
}

std::optional<std::string>
Vault::Sys::Auth::enable(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(path), parameters);
}

std::optional<std::string> Vault::Sys::Auth::disable(const Path &path) const {
  return HttpConsumer::del(client_, getUrl(path));
}

std::optional<std::string>
Vault::Sys::Auth::readTuning(const Path &path) const {
  return HttpConsumer::get(client_, getUrl(Vault::Path{path + "/tune"}));
}

std::optional<std::string>
Vault::Sys::Auth::tune(const Path &path, const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Vault::Path{path + "/tune"}),
                            parameters);
}

Vault::Url Vault::Sys::Auth::getUrl(const Path &path) const {
  return client_.getUrl("/v1/sys/auth", path.empty() ? path : Path{"/" + path});
}
