#include <utility>
#include "json.hpp"
#include "VaultClient.h"

Vault::KeyValue::KeyValue(const Vault::Client& client)
  : version_(KeyValue::Version::v2)
  , client_(client)
  , mount_("secret")
  {}

Vault::KeyValue::KeyValue(const Vault::Client& client, Vault::SecretMount mount)
  : version_(KeyValue::Version::v2)
  , client_(client)
  , mount_(std::move(mount))
  {}

Vault::KeyValue::KeyValue(const Vault::Client& client, KeyValue::Version version)
  : version_(version)
  , client_(client)
  , mount_("secret")
  {}

Vault::KeyValue::KeyValue(const Vault::Client& client, Vault::SecretMount mount, KeyValue::Version version)
  : version_(version)
  , client_(client)
  , mount_(std::move(mount))
  {}

Vault::Url Vault::KeyValue::getUrl(const Vault::Path& path) {
  return version_ == KeyValue::Version::v1
    ? client_.getUrl("/v1/" + mount_ + "/", path)
    : client_.getUrl("/v1/" + mount_ + "/data/", path);
}

Vault::Url Vault::KeyValue::getMetadataUrl(const Vault::Path& path) {
  return Vault::Url{client_.getUrl("/v1/" + mount_ + "/metadata/", path)};
}

std::optional<std::string> Vault::KeyValue::list(const Vault::Path& path) {
  return version_ == KeyValue::Version::v1
    ? Vault::HttpConsumer::list(client_, getUrl(path))
    : Vault::HttpConsumer::list(client_, getMetadataUrl(path));
}

std::optional<std::string> Vault::KeyValue::get(const Vault::Path& path) {
  return Vault::HttpConsumer::get(client_, getUrl(path));
}

std::optional<std::string> Vault::KeyValue::put(const Vault::Path& path, const Parameters& parameters) {
  return Vault::HttpConsumer::post(client_, getUrl(path), parameters, [&](const Parameters& params) {
    nlohmann::json j;
    j["data"] = nlohmann::json::object();
    std::for_each(parameters.begin(), parameters.end(),
      [&](std::pair<std::string, std::string> pair) {
        j["data"][pair.first] = pair.second;
      }
    );
    return j.dump();
  });
}

std::optional<std::string> Vault::KeyValue::del(const Vault::Path& path) {
  return Vault::HttpConsumer::del(client_, getUrl(path));
}

std::optional<std::string> Vault::KeyValue::del(const Vault::Path& path, std::vector<long> versions) {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::post(
    client_,
    client_.getUrl("/v1" + mount_ + "/delete/", path),
    Parameters{},
    [&](auto params){
      nlohmann::json j;
      j["versions"] = versions;
      return j.dump();
    }
  );
}

std::optional<std::string> Vault::KeyValue::destroy(const Vault::Path& path, std::vector<long> versions) {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::post(
    client_,
    client_.getUrl("/v1" + mount_ + "/destroy/", path),
    Parameters{},
    [&](auto params) {
      nlohmann::json j;
      j["versions"] = versions;
      return j.dump();
    }
  );
}
