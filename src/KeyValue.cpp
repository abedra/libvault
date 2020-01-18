#include <utility>
#include "json.hpp"
#include "VaultClient.h"

KeyValue::KeyValue(const VaultClient& client)
  : version_(KeyValue::Version::v2)
  , client_(client)
  , mount_("secret")
  {}

KeyValue::KeyValue(const VaultClient& client, Vault::SecretMount mount)
  : version_(KeyValue::Version::v2)
  , client_(client)
  , mount_(std::move(mount))
  {}

KeyValue::KeyValue(const VaultClient& client, KeyValue::Version version)
  : version_(version)
  , client_(client)
  , mount_("secret")
  {}

KeyValue::KeyValue(const VaultClient &client, Vault::SecretMount mount, KeyValue::Version version)
  : version_(version)
  , client_(client)
  , mount_(std::move(mount))
  {}

Vault::Url KeyValue::getUrl(const Vault::Path& path) {
  return version_ == KeyValue::Version::v1
    ? client_.getUrl("/v1/" + mount_ + "/", path)
    : client_.getUrl("/v1/" + mount_ + "/data/", path);
}

Vault::Url KeyValue::getMetadataUrl(const Vault::Path& path) {
  return Vault::Url{client_.getUrl("/v1/" + mount_ + "/metadata/", path)};
}

std::optional<std::string> KeyValue::list(const Vault::Path& path) {
  return version_ == KeyValue::Version::v1
    ? VaultHttpConsumer::list(client_, getUrl(path))
    : VaultHttpConsumer::list(client_, getMetadataUrl(path));
}

std::optional<std::string> KeyValue::get(const Vault::Path& path) {
  return VaultHttpConsumer::get(client_, getUrl(path));
}

std::optional<std::string> KeyValue::put(const Vault::Path& path, const Parameters& parameters) {
  return VaultHttpConsumer::post(client_, getUrl(path), parameters, [&](const Parameters& params) {
    nlohmann::json j;
    j["data"] = nlohmann::json::object();
    std::for_each(parameters.begin(), parameters.end(),
      [&](std::pair<std::string, std::string> pair) {
        j["data"][pair.first] = pair.second;
      }
    );
    return j;
  });
}

std::optional<std::string> KeyValue::del(const Vault::Path& path) {
  return VaultHttpConsumer::del(client_, getUrl(path));
}

std::optional<std::string> KeyValue::del(const Vault::Path& path, std::vector<long> versions) {
  if (!client_.is_authenticated() || version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  nlohmann::json j;
  j["versions"] = versions;

  auto response = client_.getHttpClient()
    .post(
      client_.getUrl("/v1" + mount_ + "/delete/", path),
      client_.getToken(),
      client_.getNamespace(),
      j.dump()
    );

  return HttpClient::is_success(response)
    ? std::optional<std::string>(response.value().body.value())
    : std::nullopt;
}

std::optional<std::string> KeyValue::destroy(const Vault::Path& path, std::vector<long> versions) {
  if (!client_.is_authenticated() || version_ != KeyValue::Version::v2) {
    return std::nullopt;
  }

  nlohmann::json j;
  j["versions"] = versions;

  auto response = client_.getHttpClient().post(
    client_.getUrl("/v1" + mount_ + "/destroy/", path),
    client_.getToken(),
    client_.getNamespace(),
    j.dump()
  );

  return HttpClient::is_success(response)
    ? std::optional<std::string>(response.value().body.value())
    : std::nullopt;
}
