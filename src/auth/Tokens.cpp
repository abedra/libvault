#include "VaultClient.h"

namespace Vault {

std::optional<std::string>  Tokens::createToken(const JsonParameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"create"}), parameters);
}

std::optional<std::string>  Tokens::lookupTokenSelf() {
  return HttpConsumer::get(client_, getUrl(Path{"lookup-self"}));
}

std::optional<std::string> Tokens::listAccessors() {
  return HttpConsumer::list(client_, getUrl(Path{"accessors"}));
}

std::optional<std::string> Tokens::revokeTokenSelf() {
  return HttpConsumer::post(client_, getUrl(Path{"revoke-self"}), Vault::Parameters{});
}

std::optional<std::string> Tokens::renewTokenSelf(const JsonParameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"renew-self"}), parameters);
}

std::optional<std::string> Tokens::tidyTokens() {
  return HttpConsumer::post(client_, getUrl(Path{"tidy"}), Vault::Parameters{});
}

std::optional<std::string> Tokens::readTokenRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Tokens::listTokenRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Tokens::createTokenRole(const Path &path, const JsonParameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roles/" + path}), parameters);
}

std::optional<std::string> Tokens::deleteTokenRole(const Path &path) {
  return HttpConsumer::del(client_,getUrl(Path{"roles/" + path}));
}

std::optional<std::string> Tokens::lookupToken(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"lookup"}), parameters);
}

std::optional<std::string> Tokens::renewToken(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"renew"}), parameters);
}

std::optional<std::string> Tokens::revokeToken(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"revoke"}), parameters);
}

std::optional<std::string> Tokens::lookupTokenAccessor(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"lookup-accessor"}), parameters);
}

std::optional<std::string> Tokens::renewTokenAccessor(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"renew-accessor"}), parameters);
}

std::optional<std::string> Tokens::revokeTokenAccessor(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"revoke-accessor"}), parameters);
}

std::optional<std::string> Tokens::revokeTokenAndOrphanChildren(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"revoke-orphan"}), parameters);
}

Url Tokens::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/token/", path);
}

std::optional<std::string> Tokens::createWrappedToken(const JsonParameters &parameters, const TTL& ttl) {
  return  HttpConsumer::post(
          client_,
          getUrl(Path{"create"}),
          parameters,
          [&](curl_slist *chunk) {
            return curl_slist_append(chunk, ("X-Vault-Wrap-TTL: " + ttl).c_str());
          }
  );
}

}  // namespace Vault