#include "VaultClient.h"

std::optional<std::string>
Vault::GoogleCloudKms::configure(const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"config"}), parameters);
}

std::optional<std::string> Vault::GoogleCloudKms::readConfiguration() {
  return HttpConsumer::get(client_, getUrl(Path{"config"}));
}

std::optional<std::string> Vault::GoogleCloudKms::deleteConfiguration() {
  return HttpConsumer::del(client_, getUrl(Path{"config"}));
}

std::optional<std::string>
Vault::GoogleCloudKms::decrypt(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"decrypt/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::GoogleCloudKms::encrypt(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"encrypt/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::GoogleCloudKms::reencrypt(const Path &path,
                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"reencrypt/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::GoogleCloudKms::sign(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"sign/" + path}), parameters);
}

std::optional<std::string>
Vault::GoogleCloudKms::verify(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"verify/" + path}),
                            parameters);
}

std::optional<std::string> Vault::GoogleCloudKms::listKeys() {
  return HttpConsumer::list(client_, getUrl(Path{"keys"}));
}

std::optional<std::string>
Vault::GoogleCloudKms::createKey(const Path &path,
                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/" + path}), parameters);
}

std::optional<std::string>
Vault::GoogleCloudKms::updateKey(const Path &path,
                                 const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/" + path}), parameters);
}

std::optional<std::string> Vault::GoogleCloudKms::deleteKey(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"keys/" + path}));
}

std::optional<std::string> Vault::GoogleCloudKms::readKey(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"keys/" + path}));
}

std::optional<std::string>
Vault::GoogleCloudKms::readKeyConfiguration(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"keys/config/" + path}));
}

std::optional<std::string>
Vault::GoogleCloudKms::updateKeyConfiguration(const Path &path,
                                              const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/config/" + path}),
                            parameters);
}

std::optional<std::string>
Vault::GoogleCloudKms::deregisterKey(const Path &path) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/deregister/" + path}),
                            Parameters{});
}

std::optional<std::string>
Vault::GoogleCloudKms::registerKey(const Path &path,
                                   const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/register/" + path}),
                            parameters);
}

std::optional<std::string> Vault::GoogleCloudKms::rotateKey(const Path &path) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/rotate/" + path}),
                            Parameters{});
}

std::optional<std::string>
Vault::GoogleCloudKms::trimKeyVersions(const Path &path) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/trim/" + path}),
                            Parameters{});
}

Vault::Url Vault::GoogleCloudKms::getUrl(const Path &path) {
  return client_.getUrl("/v1/gcpkms/", path);
}
