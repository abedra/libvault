#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

#include "VaultClient.h"
#include "HttpClient.h"

using json = nlohmann::json;

VaultClient::VaultClient(std::string host, std::string port) : VaultClient(host, port, false) {}

VaultClient::VaultClient(std::string host, std::string port, bool debug) {
  this->host = host;
  this->port = port;
  this->httpClient = HttpClient(debug);
}

void VaultClient::authenticate(std::string role_id, std::string secret_id) {
  json j;
  j = json::object();
  j["role_id"] = role_id;
  j["secret_id"] = secret_id;

  auto response = httpClient.post(getUrl("/v1/auth/approle", "/login"), token, j.dump());
  token = json::parse(response)["auth"]["client_token"];
}

std::string VaultClient::getUrl(std::string base, std::string path) {
  return "http://" + host + ":" + port + base + path;
}
