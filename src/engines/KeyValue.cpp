#include "VaultClient.h"
#include "json.hpp"
#include "support/CreateJson.h"
#include <iostream>
#include <utility>

Vault::KeyValue::KeyValue(const Vault::Client &client)
    : version_(KeyValue::Version::v2), client_(client), mount_("secret") {}

Vault::KeyValue::KeyValue(const Vault::Client &client, Vault::SecretMount mount)
    : version_(KeyValue::Version::v2), client_(client),
      mount_(std::move(mount)) {}

Vault::KeyValue::KeyValue(const Vault::Client &client,
                          KeyValue::Version version)
    : version_(version), client_(client), mount_("secret") {}

Vault::KeyValue::KeyValue(const Vault::Client &client, Vault::SecretMount mount,
                          KeyValue::Version version)
    : version_(version), client_(client), mount_(std::move(mount)) {}

Vault::Url Vault::KeyValue::getUrl(const Vault::Path &path) {
  return version_ == KeyValue::Version::v1
             ? client_.getUrl("/v1/" + mount_ + "/", path)
             : client_.getUrl("/v1/" + mount_ + "/data/", path);
}

Vault::Url Vault::KeyValue::getMetadataUrl(const Vault::Path &path) {
  return Vault::Url{client_.getUrl("/v1/" + mount_ + "/metadata/", path)};
}

std::optional<std::string> Vault::KeyValue::list(const Vault::Path &path) {
  return version_ == KeyValue::Version::v1
             ? Vault::HttpConsumer::list(client_, getUrl(path))
             : Vault::HttpConsumer::list(client_, getMetadataUrl(path));
}

std::optional<std::string> Vault::KeyValue::read(const Vault::Path &path) {
  return Vault::HttpConsumer::get(client_, getUrl(path));
}

std::optional<std::string>
Vault::KeyValue::read(const Vault::Path &path,
                      const Vault::SecretVersion &secretVersion) {
  return read(Vault::Path{path + "?version=" + secretVersion});
}

std::optional<std::string>
Vault::KeyValue::create(const Vault::Path &path, const Parameters &parameters) {
  return Vault::HttpConsumer::post(
      client_, getUrl(path), parameters, [&](const Parameters &params) {
        nlohmann::json json;
        json["data"] = helpers::create_json(params);
        return json.dump();
      });
}

std::optional<std::string>
Vault::KeyValue::update(const Vault::Path &path, const Parameters &parameters) {
  if (version_ != KeyValue::Version::v1) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::put(client_, getUrl(path), parameters,
                                  [&](const Parameters &params) {
                                    nlohmann::json json;
                                    json["data"] = helpers::create_json(params);
                                    return json.dump();
                                  });
}

std::optional<std::string> Vault::KeyValue::del(const Vault::Path &path) {
  return Vault::HttpConsumer::del(client_, getUrl(path));
}

std::optional<std::string> Vault::KeyValue::del(const Vault::Path &path,
                                                std::vector<int64_t> versions) {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::post(
      client_, client_.getUrl("/v1" + mount_ + "/delete/", path), Parameters{},
      [&](const Parameters &params) {
        nlohmann::json j;
        j["versions"] = versions;
        return j.dump();
      });
}

std::optional<std::string>
Vault::KeyValue::destroy(const Vault::Path &path,
                         std::vector<int64_t> versions) {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::post(
      client_, client_.getUrl("/v1" + mount_ + "/destroy/", path), Parameters{},
      [&](const Parameters &params) {
        nlohmann::json j;
        j["versions"] = versions;
        return j.dump();
      });
}

std::optional<std::string>
Vault::KeyValue::undelete(const Path &path, std::vector<int64_t> versions) {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::post(
      client_, client_.getUrl("/v1" + mount_ + "/undelete/", path),
      Parameters{}, [&](const Parameters &params) {
        nlohmann::json j;
        j["versions"] = versions;
        return j.dump();
      });
}

std::optional<std::string> Vault::KeyValue::readConfig() {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::get(
      client_, client_.getUrl("/v1/" + mount_, Vault::Path{"/config"}));
}

std::optional<std::string>
Vault::KeyValue::updateConfig(const Parameters &parameters) {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::post(
      client_, client_.getUrl("/v1/" + mount_, Vault::Path{"/config"}),
      parameters);
}

std::optional<std::string> Vault::KeyValue::readMetadata(const Path &path) {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::get(client_, getMetadataUrl(path));
}

std::optional<std::string>
Vault::KeyValue::updateMetadata(const Path &path,
                                const Parameters &parameters) {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::post(client_, getMetadataUrl(path), parameters);
}

std::optional<std::string> Vault::KeyValue::deleteMetadata(const Path &path) {
  if (version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  return Vault::HttpConsumer::del(client_, getMetadataUrl(path));
}
