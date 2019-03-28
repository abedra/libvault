#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

#include "VaultClient.h"

VaultClient::VaultClient(std::string host, std::string port) :
  host_(host), port_(port), debug_(false) {}

VaultClient::VaultClient(std::string host, std::string port, bool debug) :
  host_(host), port_(port), debug_(debug) {}

void VaultClient::authenticate(std::string role_id, std::string secret_id) {
  nlohmann::json j;
  j = nlohmann::json::object();
  j["role_id"] = role_id;
  j["secret_id"] = secret_id;

  auto response = httpClient_.post(getUrl("/v1/auth/approle", "/login"), token_, j.dump());
  token_ = nlohmann::json::parse(response)["auth"]["client_token"];
}

std::string VaultClient::getUrl(std::string base, std::string path) {
  return "http://" + host_ + ":" + port_ + base + path;
}
