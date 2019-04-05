#include <nlohmann/json.hpp>
#include "VaultClient.h"

Transit::Transit(const VaultClient& client) : client_(client) {}

std::string
Transit::getUrl(std::string path) {
  return client_.getUrl("/v1/transit/", path);
}

std::experimental::optional<std::string>
Transit::encrypt(std::string path, std::unordered_map<std::string, std::string> parameters) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  parameters["plaintext"] = Base64::encode(
    reinterpret_cast<const unsigned char*>(parameters["plaintext"].c_str()),
    parameters["plaintext"].length()
  );

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
Transit::decrypt(std::string path, std::unordered_map<std::string, std::string>  parameters) {
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
Transit::generate_data_key(std::string path, std::unordered_map<std::string, std::string> parameters) {
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
Transit::generate_wrapped_data_key(std::string path, std::unordered_map<std::string, std::string> parameters) {
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
Transit::generate_random_bytes(int num_bytes) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  nlohmann::json j = nlohmann::json::object();
  j["format"] = "base64";

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
Transit::generate_random_hex_bytes(int num_bytes) {
  if (!client_.is_authenticated()) {
    return std::experimental::nullopt;
  }

  nlohmann::json j = nlohmann::json::object();
  j["format"] = "hex";

  auto response = client_.getHttpClient()
    .post(getUrl("random/" + std::to_string(num_bytes)),
	  client_.getToken(),
	  client_.getNamespace(),
	  j.dump());

  return HttpClient::is_success(response) ?
    std::experimental::optional<std::string>(response.value().body) :
    std::experimental::nullopt;
}
