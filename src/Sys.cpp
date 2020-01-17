#include <iostream>
#include "VaultClient.h"

Sys::Sys(const VaultClient &client) : client_(client) { }

std::optional<std::string> Sys::health() {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client_.getHttpClient().get(
    getUrl(Vault::Path{"/health"}),
    client_.getToken(),
    client_.getNamespace()
  );

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string> Sys::health(const Vault::Url &leader) {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client_.getHttpClient().get(
    Vault::Url{leader + "/v1/sys/health"},
    client_.getToken(),
    client_.getNamespace()
  );

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string> Sys::leader() {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client_.getHttpClient().get(
    getUrl(Vault::Path{"/leader"}),
    client_.getToken(),
    client_.getNamespace()
  );

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

Vault::Url Sys::getUrl(const Vault::Path &path) {
  return client_.getUrl("/v1/sys", path);
}
