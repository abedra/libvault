#include "json.hpp"
#include "VaultClient.h"

std::optional<std::string> Vault::Transit::createKey(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/" + path}), parameters);
}

std::optional<std::string> Vault::Transit::readKey(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"keys/" + path}));
}

std::optional<std::string> Vault::Transit::deleteKey(const Path &path) {
  return HttpConsumer::del(client_, getUrl(Path{"keys/" + path}));
}

std::optional<std::string> Vault::Transit::listKeys() {
  return HttpConsumer::list(client_, getUrl(Path{"keys"}));
}

std::optional<std::string> Vault::Transit::updateKeyConfiguration(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"keys/" + path + "/config"}), parameters);
}

std::optional<std::string> Vault::Transit::rotateKey(const Path &path) {
  return HttpConsumer::post(client_,getUrl(Path{"keys/" + path + "/rotate"}),Parameters{});
}

std::optional<std::string> Vault::Transit::exportKey(const KeyType &keyType, const Path &path, const Path &version) {
  return HttpConsumer::get(client_, getUrl(Path{"export/" + keyType + "/" + path + "/" + version}));
}

std::optional<std::string> Vault::Transit::encrypt(const Path& path, const Parameters& parameters) {
  return HttpConsumer::post(client_,getUrl(Path{"encrypt/" + path}), parameters);
}

std::optional<std::string> Vault::Transit::decrypt(const Path& path, const Parameters& parameters) {
  auto response = HttpConsumer::post(client_,getUrl(Path{"decrypt/" + path}), parameters);

  if (response) {
    auto encoded_text = nlohmann::json::parse(response.value())["data"]["plaintext"];
    return std::optional<std::string>(Base64::decode(encoded_text));
  } else {
    return std::nullopt;
  }
}

std::optional<std::string> Vault::Transit::rewrap(const Path &path, const Parameters &parameters) {
  return HttpConsumer::post(client_, getUrl(Path{"rewrap/" + path}), parameters);
}

std::optional<std::string> Vault::Transit::generateDataKey(const Path& path, const Parameters& parameters) {
  return HttpConsumer::post(client_,getUrl(Path{"datakey/plaintext/" + path}), parameters);
}

std::optional<std::string> Vault::Transit::generateWrappedDataKey(const Path& path, const Parameters& parameters) {
  return HttpConsumer::post(client_,getUrl(Path{"datakey/wrapped/" + path}), parameters);
}

std::optional<std::string> Vault::Transit::generateRandomBytes(int num_bytes, const Parameters& parameters) {
  return HttpConsumer::post(client_,getUrl(Path{"random/" + std::to_string(num_bytes)}), parameters);
}

std::optional<std::string> Vault::Transit::hash(const Algorithm& algorithm, const Parameters& parameters) {
  return HttpConsumer::post(client_,getUrl(Path{"hash/" + algorithm}), parameters);
}

std::optional<std::string>
Vault::Transit::hmac(const Path& key, const Algorithm& algorithm, const Parameters& parameters) {
  return HttpConsumer::post(client_,getUrl(Path{"hmac/" + key + "/" + algorithm}), parameters);
}

std::optional<std::string>
Vault::Transit::sign(const Path& key, const Algorithm& algorithm, const Parameters& parameters) {
  return HttpConsumer::post(client_,getUrl(Path{"sign/" + key + "/" + algorithm}), parameters);
}

std::optional<std::string>
Vault::Transit::verify(const Path& key, const Algorithm& algorithm, const Parameters& parameters) {
  return HttpConsumer::post(client_,getUrl(Path{"verify/" + key + "/" + algorithm}), parameters);
}

std::optional<std::string> Vault::Transit::backup(const Path &path) {
  return HttpConsumer::get(client_, getUrl(Path{"backup/" + path}));
}

Vault::Url Vault::Transit::getUrl(const Path& path) {
  return client_.getUrl("/v1/transit/", path);
}
