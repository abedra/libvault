#include "json.hpp"
#include "VaultClient.h"

Transit::Transit(const VaultClient& client)
  : client_(client)
  {}

Url Transit::getUrl(const Path& path) {
  return client_.getUrl("/v1/transit/", path);
}

optional<std::string> Transit::encrypt(const Path& path, const Parameters& parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Path{"encrypt/" + path}),
    parameters
  );
}

optional<std::string> Transit::decrypt(const Path& path, const Parameters& parameters) {
  auto response = VaultHttpConsumer::post(
    client_,
    getUrl(Path{"decrypt/" + path}),
    parameters
  );

  if (response) {
    auto encoded_text = nlohmann::json::parse(response.value())["data"]["plaintext"];
    return optional<std::string>(Base64::decode(encoded_text));
  } else {
    return std::nullopt;
  }
}

optional<std::string> Transit::generate_data_key(const Path& path, const Parameters& parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Path{"datakey/plaintext/" + path}),
    parameters
  );
}

optional<std::string> Transit::generate_wrapped_data_key(const Path& path, const Parameters& parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Path{"datakey/wrapped/" + path}),
    parameters
  );
}

optional<std::string> Transit::generate_random_bytes(int num_bytes, const Parameters& parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Path{"random/" + std::to_string(num_bytes)}),
    parameters
  );
}

optional<std::string> Transit::hash(const Algorithm& algorithm, const Parameters& parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Path{"hash/" + algorithm}),
    parameters
  );
}

optional<std::string> Transit::hmac(const Path& key,
                                    const Algorithm& algorithm,
                                    const Parameters& parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Path{"hmac/" + key + "/" + algorithm}),
    parameters
  );
}

optional<std::string> Transit::sign(const Path& key,
                                    const Algorithm& algorithm,
                                    const Parameters& parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Path{"sign/" + key + "/" + algorithm}),
    parameters
  );
}

optional<std::string> Transit::verify(const Path& key,
                                      const Algorithm& algorithm,
                                      const Parameters& parameters) {
  return VaultHttpConsumer::post(
    client_,
    getUrl(Path{"verify/" + key + "/" + algorithm}),
    parameters
  );
}
