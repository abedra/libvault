#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

#include "VaultClient.h"
#include "HttpClient.h"

using json = nlohmann::json;

VaultClient::VaultClient(std::string host,
			 std::string port,
			 std::string token) :
  VaultClient(host, port, token, false) {}

VaultClient::VaultClient(std::string host,
			 std::string port,
			 std::string token,
			 bool debug) {
  this->host = host;
  this->port = port;
  this->token = token;
  this->httpClient = HttpClient(debug);
}

std::string VaultClient::vaultUrl(std::string path) {
  return "http://" + host + ":" + port + "/v1/secret/data/" + path;
}

std::string VaultClient::get(std::string path) {
  return httpClient.get(vaultUrl(path), token);
}

int VaultClient::put(std::string path,
		     std::unordered_map<std::string, std::string> map) {
  json j;
  j["data"] = json::object();
  std::for_each(map.begin(), map.end(), [&](auto pair) {
    j["data"][pair.first] = pair.second;
  });

  return httpClient.post(vaultUrl(path), token, j.dump());
}

int VaultClient::del(std::string path) {
  return httpClient.del(vaultUrl(path), token);
}
