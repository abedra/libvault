#include <utility>
#include "json.hpp"
#include "VaultClient.h"

KeyValue::KeyValue(const VaultClient& client)
  : version_(KeyValue::Version::v2)
  , client_(client)
  , mount_("secret")
  {}

KeyValue::KeyValue(const VaultClient& client, SecretMount mount)
  : version_(KeyValue::Version::v2)
  , client_(client)
  , mount_(std::move(mount))
  {}

KeyValue::KeyValue(const VaultClient& client, KeyValue::Version version)
  : version_(version)
  , client_(client)
  , mount_("secret")
  {}

KeyValue::KeyValue(const VaultClient &client, SecretMount mount, KeyValue::Version version)
  : version_(version)
  , client_(client)
  , mount_(std::move(mount))
  {}

Url KeyValue::getUrl(const Path& path) {
  switch (version_) {
  case KeyValue::Version::v1:
    return client_.getUrl("/v1/" + mount_ + "/", path);
  case KeyValue::Version::v2:
    return client_.getUrl("/v1/" + mount_ + "/data/", path);
  default:
    return client_.getUrl("/v1/" + mount_ + "/data/", path);
  }
}

Url KeyValue::getMetadataUrl(const Path& path) {
  return Url{client_.getUrl("/v1/" + mount_ + "/metadata/", path)};
}

optional<std::string> KeyValue::list(const Path& path) {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  optional<HttpResponse> response;

  if (version_ == KeyValue::Version::v1) {
    response = client_.getHttpClient().list(
      getUrl(path),
      client_.getToken(),
      client_.getNamespace()
    );
  } else {
    response = client_.getHttpClient().list(
      getMetadataUrl(path),
      client_.getToken(),
      client_.getNamespace()
    );
  }

  return HttpClient::is_success(response)
    ? optional<std::string>(response.value().body.value())
    : std::nullopt;
}

optional<std::string> KeyValue::get(const Path& path) {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client_.getHttpClient().get(
    getUrl(path),
    client_.getToken(),
    client_.getNamespace()
  );

  return HttpClient::is_success(response)
    ? optional<std::string>(response.value().body.value())
    : std::nullopt;
}

optional<std::string> KeyValue::put(const Path& path, Parameters parameters) {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  nlohmann::json j;
  j["data"] = nlohmann::json::object();
  std::for_each(
    parameters.begin(),
    parameters.end(),
    [&](std::pair<std::string, std::string> pair) {
      j["data"][pair.first] = pair.second;
    }
  );

  auto response = client_.getHttpClient().post(
    getUrl(path),
    client_.getToken(),
    client_.getNamespace(),
    j.dump()
  );

  return response
    ? optional<std::string>(response.value().body.value())
    : std::nullopt;
}

optional<std::string> KeyValue::del(const Path& path) {
  if (!client_.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client_.getHttpClient().del(
    getUrl(path),
    client_.getToken(),
    client_.getNamespace()
  );

  return HttpClient::is_success(response)
    ? optional<std::string>(response.value().body.value())
    : std::nullopt;
}

optional<std::string> KeyValue::del(const Path& path, std::vector<long> versions) {
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
    ? optional<std::string>(response.value().body.value())
    : std::nullopt;
}

optional<std::string> KeyValue::destroy(const Path& path, std::vector<long> versions) {
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
    ? optional<std::string>(response.value().body.value())
    : std::nullopt;
}
