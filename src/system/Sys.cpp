#include <iostream>
#include "VaultClient.h"

std::optional<std::string> Vault::Sys::health() {
  return Vault::HttpConsumer::get(client_, getUrl(Path{"health"}));
}

std::optional<std::string> Vault::Sys::health(const Url &leader) {
  return HttpConsumer::get(client_, Url{leader + "/v1/sys/health"});
}

std::optional<std::string> Vault::Sys::leader() {
  return HttpConsumer::get(client_, getUrl(Path("leader")));
}

std::optional<std::string> Vault::Sys::wrap(const Parameters &parameters, const TTL& ttl) {
  return HttpConsumer::post(
      client_,
      getUrl(Path{"wrapping/wrap"}),
      parameters,
      [&](curl_slist *chunk) {
        return curl_slist_append(chunk, ("X-Vault-Wrap-TTL: " + ttl).c_str());
      }
  );
}

std::optional<std::string>
Vault::Sys::unwrap(const Token& token) {
  auto response = client_.getHttpClient().post(
      getUrl(Path{"wrapping/unwrap"}),
      token,
      client_.getNamespace(),
      ""
  );

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string> Vault::Sys::lookup(const Token &token) {
  return HttpConsumer::post(
    client_,
    getUrl(Path{"wrapping/lookup"}),
    Parameters({{"token", token.value()}})
  );
}

std::optional<std::string> Vault::Sys::rewrap(const Token &token) {
  return HttpConsumer::post(
      client_,
      getUrl(Path{"wrapping/rewrap"}),
      Parameters({{"token", token.value()}})
  );
}

std::optional<std::string> Vault::Sys::auditHash(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"audit-hash/" + path}), parameters);
}

std::optional<std::string> Vault::Sys::capabilities(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"capabilities"}), parameters);
}

std::optional<std::string> Vault::Sys::capabilitiesAccessor(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"capabilities-accessor"}), parameters);
}

std::optional<std::string> Vault::Sys::capabilitiesSelf(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"capabilities-self"}), parameters);
}

std::optional<std::string> Vault::Sys::state() {
  return HttpConsumer::get(client_, getUrl(Path{"config/state/sanitized"}));
}

std::optional<std::string> Vault::Sys::hostInfo() {
  return HttpConsumer::get(client_, getUrl(Path{"host-info"}));
}

std::optional<std::string> Vault::Sys::mounts() {
  return HttpConsumer::get(client_, getUrl(Path{"internal/ui/mounts"}));
}

std::optional<std::string> Vault::Sys::keyStatus() {
  return HttpConsumer::get(client_, getUrl(Path{"key-status"}));
}

std::optional<std::string> Vault::Sys::metrics() {
  return HttpConsumer::get(client_, getUrl(Path{"metrics"}));
}

Vault::Url Vault::Sys::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/", path);
}
