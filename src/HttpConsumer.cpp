#include <iostream>
#include "json.hpp"
#include "VaultClient.h"

std::optional<std::string>
Vault::HttpConsumer::get(const Vault::Client &client, const Vault::Url &url) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client.getHttpClient().get(url, client.getToken(), client.getNamespace());

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string>
Vault::HttpConsumer::list(const Vault::Client &client, const Vault::Url &url) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client.getHttpClient().list(url, client.getToken(), client.getNamespace());

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<std::string>
Vault::HttpConsumer::del(const Vault::Client &client, const Vault::Url &url) {
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
Vault::HttpConsumer::post(const Vault::Client& client,
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
Vault::HttpConsumer::post(const Vault::Client& client,
                          const Vault::Url& url,
                          const Parameters& parameters,
                          const JsonProducer& jsonProducer) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client.getHttpClient().post(
    url,
    client.getToken(),
    client.getNamespace(),
    jsonProducer(parameters)
  );

  return HttpClient::is_success(response)
         ? std::optional<std::string>(response.value().body.value())
         : std::nullopt;
}

std::optional<Vault::AuthenticationResponse>
Vault::HttpConsumer::authenticate(const Vault::Client& client,
                                  const Vault::Url& url,
                                  const NoArgJsonProducer& jsonProducer) {
  auto response = client.getHttpClient().post(
    url,
    client.getToken(),
    client.getNamespace(),
    jsonProducer()
  );

  if (HttpClient::is_success(response)) {
    auto body = Vault::HttpResponseBodyString{response.value().body};
    auto token = Vault::Token{nlohmann::json::parse(body.value())["auth"]["client_token"]};

    return AuthenticationResponse{body, token};
  } else {
    return std::nullopt;
  }
}
