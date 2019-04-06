#include <nlohmann/json.hpp>
#include "VaultClient.h"

Transit::Transit(const VaultClient& client) : client_(client) {}

std::string
Transit::getUrl(std::string path) {
  return client_.getUrl("/v1/transit/", path);
}

std::experimental::optional<std::string>
Transit::encrypt(std::string path, Parameters parameters) {
  return VaultHttpConsumer::post(client_,
                                 getUrl("encrypt/" + path),
                                 parameters);
}

std::experimental::optional<std::string>
Transit::decrypt(std::string path, Parameters parameters) {
  auto response =  VaultHttpConsumer::post(client_,
                                           getUrl("decrypt/" + path),
                                           parameters);
  if (response) {
    auto encoded_text = nlohmann::json::parse(response.value())["data"]["plaintext"];
    return std::experimental::optional<std::string>(Base64::decode(encoded_text));
  } else {
    return std::experimental::nullopt;
  }
}

std::experimental::optional<std::string>
Transit::generate_data_key(std::string path, Parameters parameters) {
  return VaultHttpConsumer::post(client_,
                                 getUrl("datakey/plaintext/" + path),
                                 parameters);
}

std::experimental::optional<std::string>
Transit::generate_wrapped_data_key(std::string path, Parameters parameters) {
  return VaultHttpConsumer::post(client_,
                                 getUrl("datakey/wrapped/" + path),
                                 parameters);
}

std::experimental::optional<std::string>
Transit::generate_random_bytes(int num_bytes, Parameters parameters) {
  return VaultHttpConsumer::post(client_,
                                 getUrl("random/" + std::to_string(num_bytes)),
                                 parameters);
}

std::experimental::optional<std::string>
Transit::hash(std::string algorithm, Parameters parameters) {
  return VaultHttpConsumer::post(client_,
                                 getUrl("hash/" + algorithm),
                                 parameters);
}

std::experimental::optional<std::string>
Transit::hmac(std::string key, std::string algorithm, Parameters parameters) {
  return VaultHttpConsumer::post(client_,
                                 getUrl("hmac/" + key + "/" + algorithm),
                                 parameters);
}

std::experimental::optional<std::string>
Transit::sign(std::string key, std::string algorithm, Parameters parameters) {
  return VaultHttpConsumer::post(client_,
				 getUrl("sign/" + key + "/" + algorithm),
				 parameters);
}

std::experimental::optional<std::string>
Transit::verify(std::string key, std::string algorithm, Parameters parameters) {
  return VaultHttpConsumer::post(client_,
                                 getUrl("verify/" + key + "/" + algorithm),
                                 parameters);
}
