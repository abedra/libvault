#pragma once

#include <unordered_map>
#include "HttpClient.h"

class VaultClient {
private:
  std::string host;
  std::string port;
  std::string token;
  bool debug;
  HttpClient httpClient = (bool)nullptr;
  std::string vaultUrl(std::string base, std::string path);
public:
  VaultClient(std::string host, std::string port);
  VaultClient(std::string host, std::string port, bool debug);
  void authenticate(std::string role_id, std::string secret_id);
  std::string get(std::string path);
  std::string put(std::string path, std::unordered_map<std::string, std::string> map);
  std::string del(std::string path);
};
