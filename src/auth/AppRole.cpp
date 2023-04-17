#include "VaultClient.h"

std::optional<std::string> Vault::AppRole::list() const {
  return HttpConsumer::list(client_, getUrl(Path{}));
}

std::optional<std::string>
Vault::AppRole::create(const Path &roleName,
                       const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(roleName), parameters);
}

std::optional<std::string>
Vault::AppRole::update(const Path &roleName,
                       const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(roleName), parameters);
}

std::optional<std::string> Vault::AppRole::read(const Path &roleName) const {
  return HttpConsumer::get(client_, getUrl(roleName));
}

std::optional<std::string> Vault::AppRole::del(const Path &roleName) const {
  return HttpConsumer::del(client_, getUrl(roleName));
}

std::optional<std::string>
Vault::AppRole::getRoleId(const Path &roleName) const {
  return HttpConsumer::get(client_, getUrl(Path{roleName + "/role-id"}));
}

std::optional<std::string>
Vault::AppRole::updateRoleId(const Path &roleName,
                             const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{roleName + "/role-id"}),
                            parameters);
}

std::optional<std::string>
Vault::AppRole::getRoleProperty(const Path &roleName,
                                const Path &propertyName) const {
  return HttpConsumer::get(client_, getUrl(Path{roleName + propertyName}));
}

std::optional<std::string>
Vault::AppRole::updateRoleProperty(const Path &roleName,
                                   const Path &propertyName,
                                   const Parameters &parameters) const {
  return HttpConsumer::post(client_, getUrl(Path{roleName + propertyName}),
                            parameters);
}

std::optional<std::string>
Vault::AppRole::deleteRoleProperty(const Path &roleName,
                                   const Path &propertyName) const {
  return HttpConsumer::del(client_, getUrl(Path{roleName + propertyName}));
}

std::optional<std::string>
Vault::AppRole::generateSecretId(const Path &roleName,
                                 const Parameters &parameters) const {
  return Vault::HttpConsumer::post(
      client_, getUrl(Path{roleName + "/secret-id"}), parameters);
}

std::optional<std::string>
Vault::AppRole::listSecretAccessors(const Path &roleName) const {
  return HttpConsumer::list(client_, getUrl(Path{roleName + "/secret-id"}));
}

std::optional<std::string>
Vault::AppRole::readSecretId(const Path &roleName,
                             const Parameters &parameters) const {
  return HttpConsumer::post(
      client_, getUrl(Path{roleName + "/secret-id/lookup"}), parameters);
}

std::optional<std::string>
Vault::AppRole::destroySecretId(const Path &roleName,
                                const Parameters &parameters) const {
  return HttpConsumer::post(
      client_, getUrl(Path{roleName + "/secret-id/destroy"}), parameters);
}

std::optional<std::string>
Vault::AppRole::readSecretIdAccessor(const Path &roleName,
                                     const Parameters &parameters) const {
  return HttpConsumer::post(
      client_, getUrl(Path{roleName + "/secret-id-accessor/lookup"}),
      parameters);
}

std::optional<std::string>
Vault::AppRole::destroySecretIdAccessor(const Path &roleName,
                                        const Parameters &parameters) const {
  return HttpConsumer::post(
      client_, getUrl(Path{roleName + "/secret-id-accessor/destroy"}),
      parameters);
}

std::optional<std::string>
Vault::AppRole::customSecretId(const Path &roleName,
                               const Parameters &parameters) const {
  return HttpConsumer::post(
      client_, getUrl(Path{roleName + "/custom-secret-id"}), parameters);
}

std::optional<std::string> Vault::AppRole::tidy(const Path &roleName) const {
  return HttpConsumer::post(client_, getUrl(Path{roleName + "/secret-id"}),
                            Parameters{});
}

Vault::Url Vault::AppRole::getUrl(const Path &path) const {
  return client_.getUrl("/v1/auth/" + mount_ + "/role/", path);
}
