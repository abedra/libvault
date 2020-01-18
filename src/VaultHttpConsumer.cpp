#include <iostream>
#include "json.hpp"
#include "VaultClient.h"

std::optional<std::string>
VaultHttpConsumer::get(const VaultClient &client, const Vault::Url &url) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client.getHttpClient().get(url, client.getToken(), client.getNamespace());

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string>
VaultHttpConsumer::list(const VaultClient &client, const Vault::Url &url) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client.getHttpClient().list(url, client.getToken(), client.getNamespace());

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string>
VaultHttpConsumer::del(const VaultClient &client, const Vault::Url &url) {
    if (!client.is_authenticated()) {
        return std::nullopt;
    }

    auto response = client.getHttpClient().del(
            url,
            client.getToken(),
            client.getNamespace()
    );

    return HttpClient::is_success(response)
           ? std::optional<std::string>(response.value().body.value())
           : std::nullopt;
}

std::optional<std::string>
VaultHttpConsumer::post(const VaultClient& client,
                        const Vault::Url& url,
                        Parameters parameters) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  nlohmann::json j = nlohmann::json::object();
  std::for_each(
    parameters.begin(),
    parameters.end(),
    [&](std::pair<std::string, std::string> pair) {
      j[pair.first] = pair.second;
    }
  );

  auto response = client.getHttpClient().post(
      url,
      client.getToken(),
      client.getNamespace(),
      j.dump()
  );

  return HttpClient::is_success(response)
    ? std::optional<std::string>(response.value().body.value())
    : std::nullopt;
}

std::optional<std::string>
VaultHttpConsumer::post(const VaultClient& client,
                        const Vault::Url& url,
                        const Parameters& parameters,
                        const JsonProducer& jsonProducer) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  nlohmann::json j = jsonProducer(parameters);

  auto response = client.getHttpClient().post(
    url,
    client.getToken(),
    client.getNamespace(),
    j.dump()
  );

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<AuthenticationResponse>
VaultHttpConsumer::authenticate(const VaultClient& client,
                                const Vault::Url& url,
                                const NoArgJsonProducer& jsonProducer) {
  nlohmann::json j = jsonProducer();

  auto response = client.getHttpClient().post(
    url,
    client.getToken(),
    client.getNamespace(),
    j.dump()
  );

  if (HttpClient::is_success(response)) {
    auto body = Vault::HttpResponseBodyString{response.value().body};
    auto token = Vault::Token{nlohmann::json::parse(body.value())["auth"]["client_token"]};

    return AuthenticationResponse{body, token};
  } else {
    return std::nullopt;
  }
}
