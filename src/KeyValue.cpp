#include <nlohmann/json.hpp>
#include "VaultClient.h"

KeyValue::KeyValue(const VaultClient& client) :
  version_(KeyValue::Version::v2),
  client_(client),
  mount_("/secret")
{}

KeyValue::KeyValue(const VaultClient& client,
		   std::string mount) :
  version_(KeyValue::Version::v2),
  client_(client),
  mount_(mount)
{}

KeyValue::KeyValue(const VaultClient& client,
		   KeyValue::Version version) :
  version_(version),
  client_(client),
  mount_("/secret")
{}

KeyValue::KeyValue(const VaultClient &client,
		   std::string mount,
		   KeyValue::Version version) :
  version_(version),
  client_(client),
  mount_(mount)
{}

std::string
KeyValue::getUrl(std::string path) {
  switch (version_) {
  case KeyValue::Version::v1:
    return client_.getUrl("/v1" + mount_ + "/", path);
  case KeyValue::Version::v2:
    return client_.getUrl("/v1" + mount_ + "/data/", path);
  default:
    return client_.getUrl("/v1" + mount_ + "/data/", path);
  }
}

std::string
KeyValue::getMetadataUrl(std::string path) {
  return client_.getUrl("/v1" + mount_ + "/metadata/", path);
}

optional<std::string>
KeyValue::list(std::string path) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  optional<HttpResponse> response;

  if (version_ == KeyValue::Version::v1) {
    response = client_.getHttpClient()
      .list(getUrl(path), client_.getToken(), client_.getNamespace());
  } else {
    response = client_.getHttpClient()
      .list(getMetadataUrl(path), client_.getToken(), client_.getNamespace());
  }

  return HttpClient::is_success(response) ?
    optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

optional<std::string>
KeyValue::get(std::string path) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  auto response = client_.getHttpClient()
    .get(getUrl(path), client_.getToken(), client_.getNamespace());

  return HttpClient::is_success(response) ?
    optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

optional<std::string>
KeyValue::put(std::string path,
	      std::unordered_map<std::string, std::string> map) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  nlohmann::json j;
  j["data"] = nlohmann::json::object();
  std::for_each(map.begin(), map.end(),
		[&](std::pair<std::string, std::string> pair) {
    j["data"][pair.first] = pair.second;
  });

  auto response = client_.getHttpClient()
    .post(getUrl(path), client_.getToken(), client_.getNamespace(), j.dump());

  return response ?
    optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

optional<std::string>
KeyValue::del(std::string path) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  auto response = client_.getHttpClient()
    .del(getUrl(path), client_.getToken(), client_.getNamespace());

  return HttpClient::is_success(response) ?
    optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

optional<std::string>
KeyValue::del(std::string path, std::vector<long> versions) {
  if (!client_.is_authenticated() || version_ != KeyValue::Version::v2) {
    return std::experimental::nullopt;
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

  return HttpClient::is_success(response) ?
    optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

optional<std::string>
KeyValue::destroy(std::string path, std::vector<long> versions) {
  if (!client_.is_authenticated() || version_ != KeyValue::Version::v2) {
    return std::experimental::nullopt;
  }

  nlohmann::json j;
  j["versions"] = versions;

  auto response = client_.getHttpClient().post(
    client_.getUrl("/v1" + mount_ + "/destroy/", path),
    client_.getToken(),
    client_.getNamespace(),
    j.dump()
  );

  return HttpClient::is_success(response) ?
    optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}
