#include "VaultClient.h"

std::optional<Vault::AuthenticationResponse>
Vault::TlsStrategy::authenticate(const Client &client) {
  return HttpConsumer::authenticate(client, getUrl(client), cert_, key_);
}

Vault::Url Vault::TlsStrategy::getUrl(const Vault::Client &client) {
  return client.getUrl("/v1/auth/cert/login", Path{});
}
