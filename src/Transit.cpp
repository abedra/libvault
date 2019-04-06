#include <nlohmann/json.hpp>
#include "VaultClient.h"

Transit::Transit(const VaultClient& client) : client_(client) {}

std::string
Transit::getUrl(std::string path) {
  return client_.getUrl("/v1/transit/", path);
}

std::experimental::optional<std::string>
Transit::encrypt(std::string path, Parameters parameters) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  // TODO: should it be encoded by default?
  parameters["plaintext"] = Base64::encode(parameters["plaintext"]);

  nlohmann::json j = nlohmann::json::object();
  std::for_each(parameters.begin(), parameters.end(),
		[&](std::pair<std::string, std::string> pair) {
    j[pair.first] = pair.second;
  });

  auto response = client_.getHttpClient()
    .post(getUrl("encrypt/" + path),
	  client_.getToken(),
	  client_.getNamespace(),
	  j.dump());

  return HttpClient::is_success(response) ?
    std::experimental::optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

std::experimental::optional<std::string>
Transit::decrypt(std::string path, Parameters parameters) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  nlohmann::json j = nlohmann::json::object();
  std::for_each(parameters.begin(), parameters.end(),
		[&](std::pair<std::string, std::string> pair) {
    j[pair.first] = pair.second;
  });

  auto response = client_.getHttpClient()
    .post(getUrl("decrypt/" + path),
	  client_.getToken(),
	  client_.getNamespace(),
	  j.dump());

  if (HttpClient::is_success(response)) {
    auto encoded_text = nlohmann::json::parse(response.value().body)["data"]["plaintext"];
    return std::experimental::optional<std::string>(Base64::decode(encoded_text));
  } else {
    return std::experimental::nullopt;
  }
}

std::experimental::optional<std::string>
Transit::generate_data_key(std::string path, Parameters parameters) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  nlohmann::json j = nlohmann::json::object();
  std::for_each(parameters.begin(), parameters.end(),
		[&](std::pair<std::string, std::string> pair) {
    j[pair.first] = pair.second;
  });

  auto response = client_.getHttpClient()
    .post(getUrl("datakey/plaintext/" + path),
	  client_.getToken(),
	  client_.getNamespace(),
	  j.dump());

  return HttpClient::is_success(response) ?
    std::experimental::optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

std::experimental::optional<std::string>
Transit::generate_wrapped_data_key(std::string path, Parameters parameters) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  nlohmann::json j = nlohmann::json::object();
  std::for_each(parameters.begin(), parameters.end(),
		[&](std::pair<std::string, std::string> pair) {
    j[pair.first] = pair.second;
  });

  auto response = client_.getHttpClient()
    .post(getUrl("datakey/wrapped/" + path),
	  client_.getToken(),
	  client_.getNamespace(),
	  j.dump());

  return HttpClient::is_success(response) ?
    std::experimental::optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

std::experimental::optional<std::string>
Transit::generate_random_bytes(int num_bytes, Parameters parameters) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  nlohmann::json j = nlohmann::json::object();
  std::for_each(parameters.begin(), parameters.end(),
		[&](std::pair<std::string, std::string> pair) {
    j[pair.first] = pair.second;
  });

  auto response = client_.getHttpClient()
    .post(getUrl("random/" + std::to_string(num_bytes)),
	  client_.getToken(),
	  client_.getNamespace(),
	  j.dump());

  return HttpClient::is_success(response) ?
    std::experimental::optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

std::experimental::optional<std::string>
Transit::hash(std::string algorithm, Parameters parameters) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  nlohmann::json j = nlohmann::json::object();
  std::for_each(parameters.begin(), parameters.end(),
		[&](std::pair<std::string, std::string> pair) {
    j[pair.first] = pair.second;
  });

  auto response = client_.getHttpClient()
    .post(getUrl("hash/" + algorithm),
	  client_.getToken(),
	  client_.getNamespace(),
	  j.dump());

  return HttpClient::is_success(response) ?
    std::experimental::optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}

std::experimental::optional<std::string>
Transit::hmac(std::string key, std::string algorithm, Parameters parameters) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  nlohmann::json j = nlohmann::json::object();
  std::for_each(parameters.begin(), parameters.end(),
		[&](std::pair<std::string, std::string> pair) {
    j[pair.first] = pair.second;
  });

  auto response = client_.getHttpClient()
    .post(getUrl("hmac/" + key + "/" + algorithm),
	  client_.getToken(),
	  client_.getNamespace(),
	  j.dump());

  return HttpClient::is_success(response) ?
    std::experimental::optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}
