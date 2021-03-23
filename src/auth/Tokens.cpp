#include "VaultClient.h"

namespace Vault {

std::optional<std::string>  Tokens::createToken(const JsonParameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"create"}), parameters);
}

Url Tokens::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/token/", path);
}

}  // namespace Vault