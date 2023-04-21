#include "VaultClient.h"

std::optional<std::string>
Vault::Sys::Leases::read(const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"lookup"}), parameters);
}

std::optional<std::string> Vault::Sys::Leases::list(const Path &path) {
  return HttpConsumer::list(client_, getUrl(Path{"lookup/" + path}));
}

std::optional<std::string>
Vault::Sys::Leases::renew(const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"renew"}), parameters);
}

std::optional<std::string>
Vault::Sys::Leases::revoke(const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"revoke"}), parameters);
}

std::optional<std::string>
Vault::Sys::Leases::revokeForce(const Path &path,
                                const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"revoke-force/" + path}),
                           parameters);
}

std::optional<std::string>
Vault::Sys::Leases::revokePrefix(const Path &path,
                                 const Parameters &parameters) {
  return HttpConsumer::put(client_, getUrl(Path{"revoke-prefix/" + path}),
                           parameters);
}

std::optional<std::string>
Vault::Sys::Leases::tidy(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"tidy"}), parameters);
}

Vault::Url Vault::Sys::Leases::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/leases/", path);
}
