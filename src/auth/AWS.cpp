#include "VaultClient.h"

std::optional<std::string>
Vault::AWS::Auth::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config/client"}), parameters);
}

std::optional<std::string> Vault::AWS::Auth::readConfig() {
  return HttpConsumer::get(client_, getUrl(Path{"config/client"}));
}

std::optional<std::string> Vault::AWS::Auth::deleteConfig() {
  return HttpConsumer::del(client_, getUrl(Path{"config/client"}));
}

std::optional<std::string> Vault::AWS::Auth::rotateRoot() {
  return HttpConsumer::post(client_, getUrl(Path{"config/rotate-root"}),
                            Parameters{});
}

std::optional<std::string>
Vault::AWS::Auth::configureIdentity(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config/identity"}),
                            parameters);
}

std::optional<std::string> Vault::AWS::Auth::readIdentityConfiguration() {
  return HttpConsumer::get(client_, getUrl(Path{"config/identity"}));
}

std::optional<std::string>
Vault::AWS::Auth::createCertificateConfiguration(const Path &path,
                                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config/certificate/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::AWS::Auth::readCertificateConfiguration(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"config/certificate/" + path}));
}

std::optional<std::string>
Vault::AWS::Auth::deleteCertificateConfiguration(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"config/certificate/" + path}));
}

std::optional<std::string> Vault::AWS::Auth::listCertificateConfigurations() {
  return HttpConsumer::list(client_, getUrl(Path{"config/certificates"}));
}

std::optional<std::string>
Vault::AWS::Auth::createSTSRole(const Path &path,
                                const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config/sts/" + path}),
                            parameters);
}

std::optional<std::string> Vault::AWS::Auth::readSTSRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"config/sts/" + path}));
}

std::optional<std::string> Vault::AWS::Auth::listSTSRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"config/sts"}));
}

std::optional<std::string> Vault::AWS::Auth::deleteSTSRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"config/sts/" + path}));
}

std::optional<std::string>
Vault::AWS::Auth::configureIdentityWhitelistTidy(const Parameters &parameters) {
  return HttpConsumer::post(
      client_, getUrl(Path{"config/tidy/identity-whitelist"}), parameters);
}

std::optional<std::string> Vault::AWS::Auth::readIdentityWhitelistTidy() {
  return HttpConsumer::get(client_,
                           getUrl(Path{"config/tidy/identity-whitelist"}));
}

std::optional<std::string> Vault::AWS::Auth::deleteIdentityWhitelistTidy() {
  return HttpConsumer::del(client_,
                           getUrl(Path{"config/tidy/identity-whitelist"}));
}

std::optional<std::string>
Vault::AWS::Auth::configureRoleTagBlacklistTidy(const Parameters &parameters) {
  return HttpConsumer::post(
      client_, getUrl(Path{"config/tidy/roletag-blacklist"}), parameters);
}

std::optional<std::string> Vault::AWS::Auth::readRoleTagBlacklistTidy() {
  return HttpConsumer::get(client_,
                           getUrl(Path{"config/tidy/roletag-blacklist"}));
}

std::optional<std::string> Vault::AWS::Auth::deleteRoleTagBlacklistTidy() {
  return HttpConsumer::del(client_,
                           getUrl(Path{"config/tidy/roletag-blacklist"}));
}

std::optional<std::string>
Vault::AWS::Auth::crateRole(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path}), parameters);
}

std::optional<std::string> Vault::AWS::Auth::readRole(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string> Vault::AWS::Auth::listRoles() {
  return HttpConsumer::list(client_, getUrl(Path{"roles"}));
}

std::optional<std::string> Vault::AWS::Auth::deleteRole(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"role/" + path}));
}

std::optional<std::string>
Vault::AWS::Auth::createRoleTags(const Path &path,
                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"role/" + path + "/tag"}),
                            parameters);
}

std::optional<std::string>
Vault::AWS::Auth::login(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"login"}), parameters);
}

std::optional<std::string>
Vault::AWS::Auth::blacklistRoleTag(const Path &path,
                                   const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"roletag-blacklist/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::AWS::Auth::readRoleTagBlacklist(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"roletag-blacklist/" + path}));
}

std::optional<std::string> Vault::AWS::Auth::listBlacklistTags() {
  return HttpConsumer::list(client_, getUrl(Path{"roletag-blacklist"}));
}

std::optional<std::string>
Vault::AWS::Auth::deleteBlacklistTags(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"roletag-blacklist/" + path}));
}

std::optional<std::string>
Vault::AWS::Auth::tidyBlacklistTags(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"tidy/roletag-blacklist"}),
                            parameters);
}

std::optional<std::string>
Vault::AWS::Auth::readIdentityWhitelist(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"identity-whitelist/" + path}));
}

std::optional<std::string> Vault::AWS::Auth::listIdentityWhitelist() {
  return HttpConsumer::list(client_, getUrl(Path{"identity-whitelist"}));
}

std::optional<std::string>
Vault::AWS::Auth::deleteIdentityWhitelist(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"identity-whitelist/" + path}));
}

std::optional<std::string>
Vault::AWS::Auth::tidyIdentityWhitelist(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"tidy/identity-whitelist"}),
                            parameters);
}

Vault::Url Vault::AWS::Auth::getUrl(const Path &path) {
  return client_.getUrl("/v1/auth/aws/", path);
}
