#include "VaultClient.h"

namespace Vault {

std::optional<std::string>  Tokens::createToken(const JsonParameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"create"}), parameters);
}

std::optional<std::string>  Tokens::lookupTokenSelf() {
  return HttpConsumer::get(client_, getUrl(Path{"lookup-self"}));
}

Url Tokens::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/token/", path);
}

std::optional<std::string> Tokens::listAccessors() {
  return HttpConsumer::list(client_, getUrl(Path{"accessors"}));
}

std::optional<std::string> Tokens::revokeTokenSelf() {
  return HttpConsumer::post(client_, getUrl(Path{"revoke-self"}), Vault::Parameters{});
}

}  // namespace Vault