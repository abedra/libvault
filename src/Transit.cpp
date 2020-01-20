#include "json.hpp"
#include "VaultClient.h"

Vault::Transit::Transit(const Client& client)
  : client_(client)
  {}

Vault::Url Vault::Transit::getUrl(const Vault::Path& path) {
  return client_.getUrl("/v1/transit/", path);
}

std::optional<std::string>
Vault::Transit::encrypt(const Vault::Path& path, const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"encrypt/" + path}),
    parameters
  );
}

std::optional<std::string>
Vault::Transit::decrypt(const Vault::Path& path, const Parameters& parameters) {
  auto response = Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"decrypt/" + path}),
    parameters
  );

  if (response) {
    auto encoded_text = nlohmann::json::parse(response.value())["data"]["plaintext"];
    return std::optional<std::string>(Base64::decode(encoded_text));
  } else {
    return std::nullopt;
  }
}

std::optional<std::string>
Vault::Transit::generate_data_key(const Vault::Path& path, const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"datakey/plaintext/" + path}),
    parameters
  );
}

std::optional<std::string>
Vault::Transit::generate_wrapped_data_key(const Vault::Path& path, const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"datakey/wrapped/" + path}),
    parameters
  );
}

std::optional<std::string>
Vault::Transit::generate_random_bytes(int num_bytes, const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"random/" + std::to_string(num_bytes)}),
    parameters
  );
}

std::optional<std::string>
Vault::Transit::hash(const Vault::Algorithm& algorithm, const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"hash/" + algorithm}),
    parameters
  );
}

std::optional<std::string>
Vault::Transit::hmac(const Vault::Path& key,
                     const Vault::Algorithm& algorithm,
                     const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"hmac/" + key + "/" + algorithm}),
    parameters
  );
}

std::optional<std::string>
Vault::Transit::sign(const Vault::Path& key,
                     const Vault::Algorithm& algorithm,
                     const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"sign/" + key + "/" + algorithm}),
    parameters
  );
}

std::optional<std::string>
Vault::Transit::verify(const Vault::Path& key,
                       const Vault::Algorithm& algorithm,
                       const Parameters& parameters) {
  return Vault::HttpConsumer::post(
    client_,
    getUrl(Vault::Path{"verify/" + key + "/" + algorithm}),
    parameters
  );
}
