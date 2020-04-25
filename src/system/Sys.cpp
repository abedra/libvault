#include <iostream>
#include "VaultClient.h"
#include "json.hpp"

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

std::optional<std::string>
Vault::Sys::unwrap(const Vault::Token& token) {
  auto response = client_.getHttpClient().post(
    getUrl(Vault::Path{"/wrapping/unwrap"}),
    token,
    client_.getNamespace(),
    ""
  );

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string> Vault::Sys::lookup(const Token &token) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"/wrapping/lookup"}),
    Parameters({{"token", token.value()}})
  );
}

std::optional<std::string> Vault::Sys::rewrap(const Token &token) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"/wrapping/rewrap"}),
    Parameters({{"token", token.value()}})
  );
}
