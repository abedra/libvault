#include <iostream>
#include "json.hpp"
#include "VaultClient.h"

std::optional<std::string>
Vault::HttpConsumer::get(const Vault::Client &client, const Vault::Url &url) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client.getHttpClient().get(url, client.getToken(), client.getNamespace());

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}

std::optional<std::string>
Vault::HttpConsumer::list(const Vault::Client &client, const Vault::Url &url) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client.getHttpClient().list(url, client.getToken(), client.getNamespace());

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
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

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}

std::optional<std::string>
Vault::HttpConsumer::post(const Vault::Client &client,
                          const Vault::Url &url,
                          const Vault::Parameters &parameters,
                          const Vault::CurlHeaderCallback &headerCallback) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  nlohmann::json json = nlohmann::json::object();
  for (auto&&[key, value] : parameters) {
    json[key] = value;
  }

  auto response = client.getHttpClient().post(
      url,
      client.getToken(),
      client.getNamespace(),
      json.dump(),
      headerCallback
  );

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}

std::optional<std::string>
Vault::HttpConsumer::post(const Vault::Client &client,
                          const Vault::Url &url,
                          const Parameters &parameters) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  nlohmann::json json = nlohmann::json::object();
  for (auto&&[key, value] : parameters) {
    json[key] = value;
  }

  auto response = client.getHttpClient().post(
      url,
      client.getToken(),
      client.getNamespace(),
      json.dump()
  );

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}

std::optional<std::string>
Vault::HttpConsumer::post(const Vault::Client &client,
                          const Vault::Url &url,
                          const Parameters &parameters,
                          const Parameters &options,
                          const Parameters &config) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  nlohmann::json json = nlohmann::json::object();
  for (auto&&[key, value] : parameters) {
    json[key] = value;
  }

  for (auto&&[key, value] : options) {
    json["options"][key] = value;
  }

  for (auto&&[key, value] : config) {
    json["config"][key] = value;
  }

  auto response = client.getHttpClient().post(
      url,
      client.getToken(),
      client.getNamespace(),
      json.dump());

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}

std::optional<std::string>
Vault::HttpConsumer::post(const Vault::Client &client,
                          const Vault::Url &url,
                          const Parameters &parameters,
                          const JsonProducer &jsonProducer) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client.getHttpClient().post(
      url,
      client.getToken(),
      client.getNamespace(),
      jsonProducer(parameters)
  );

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}


std::optional<std::string>
Vault::HttpConsumer::put(const Vault::Client &client,
                         const Vault::Url &url,
                         const Parameters &parameters) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  nlohmann::json json = nlohmann::json::object();
  for (auto&&[key, value] : parameters) {
    json[key] = value;
  }

  auto response = client.getHttpClient().put(
      url,
      client.getToken(),
      client.getNamespace(),
      json.dump()
  );

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}

std::optional<std::string>
Vault::HttpConsumer::put(const Vault::Client &client,
                         const Vault::Url &url,
                         const Parameters &parameters,
                         const Parameters &options) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  nlohmann::json json = nlohmann::json::object();
  for (auto&&[key, value] : parameters) {
    json[key] = value;
  }

  for (auto && [key, value] : options) {
    json["options"][key] = value;
  }

  auto response = client.getHttpClient().put(
      url,
      client.getToken(),
      client.getNamespace(),
      json.dump()
  );

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}

std::optional<std::string>
Vault::HttpConsumer::put(const Client &client,
                         const Url &url,
                         const Parameters &parameters,
                         const JsonProducer &jsonProducer) {
  if (!client.is_authenticated()) {
    return std::nullopt;
  }

  auto response = client.getHttpClient().put(
      url,
      client.getToken(),
      client.getNamespace(),
      jsonProducer(parameters)
  );

  if (HttpClient::is_success(response)) {
    return std::optional<std::string>(response.value().body.value());
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}

std::optional<Vault::AuthenticationResponse>
Vault::HttpConsumer::authenticate(const Vault::Client &client,
                                  const Vault::Url &url,
                                  const NoArgJsonProducer &jsonProducer) {
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
  }

  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}

std::optional<Vault::AuthenticationResponse>
Vault::HttpConsumer::authenticate(const Vault::Client &client,
                                  const Vault::Url &url,
                                  const Certificate &cert,
                                  const Certificate &key) {

  auto response = client.getHttpClient().post(url, cert, key, client.getNamespace());

  if (HttpClient::is_success(response)) {
    auto body = Vault::HttpResponseBodyString{response.value().body};
    auto token = Vault::Token{nlohmann::json::parse(body.value())["auth"]["client_token"]};

    return AuthenticationResponse{body, token};
  }


  if (response) {
    client.getHttpClient().responseErrorCallback(response.value());
  }

  return std::nullopt;
}
