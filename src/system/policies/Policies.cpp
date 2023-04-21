#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Policies::list(const Client &client,
                                                      const Url &url) {
  return HttpConsumer::list(client, url);
}

std::optional<std::string> Vault::Sys::Policies::read(const Client &client,
                                                      const Url &url) {
  return HttpConsumer::get(client, url);
}

std::optional<std::string>
Vault::Sys::Policies::create(const Client &client, const Url &url,
                             const Parameters &parameters) {
  return HttpConsumer::put(client, url, parameters);
}

std::optional<std::string>
Vault::Sys::Policies::update(const Client &client, const Url &url,
                             const Parameters &parameters) {
  return HttpConsumer::put(client, url, parameters);
}

std::optional<std::string> Vault::Sys::Policies::del(const Client &client,
                                                     const Url &url) {
  return HttpConsumer::del(client, url);
}
