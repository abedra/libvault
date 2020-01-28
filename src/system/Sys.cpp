#include <iostream>
#include "VaultClient.h"

Vault::Sys::Sys(const Vault::Client &client) : client_(client) { }

std::optional<std::string> Vault::Sys::health() {
  return Vault::HttpConsumer::get(client_, getUrl(Vault::Path{"/health"}));
}

std::optional<std::string> Vault::Sys::health(const Vault::Url &leader) {
  return Vault::HttpConsumer::get(client_, Vault::Url{leader + "/v1/sys/health"});
}

std::optional<std::string> Vault::Sys::leader() {
  return Vault::HttpConsumer::get(client_, getUrl(Vault::Path("/leader")));
}

Vault::Url Vault::Sys::getUrl(const Vault::Path &path) {
  return client_.getUrl("/v1/sys", path);
}

std::optional<std::string> Vault::Sys::wrap(const Parameters &parameters, const Vault::TTL& ttl) {
  return Vault::HttpConsumer::post(
      client_,
      getUrl(Vault::Path{"/wrapping/wrap"}),
      parameters,
      [&](curl_slist *chunk) {
        return curl_slist_append(chunk, ("X-Vault-Wrap-TTL: " + ttl).c_str());
      }
  );
}
