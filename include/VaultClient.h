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
public:
  VaultClient(std::string host, std::string port);
  VaultClient(std::string host, std::string port, bool debug);
  void authenticate(std::string role_id, std::string secret_id);
  std::string getToken() { return token; }
  HttpClient getHttpClient() { return httpClient; }
  std::string getUrl(std::string base, std::string path);
};

class KeyValue {
public:
  enum Version { v1, v2 };

  KeyValue(VaultClient& client);
  KeyValue(VaultClient& client, std::string mount);
  KeyValue(VaultClient& client, KeyValue::Version version);
  KeyValue(VaultClient& client, std::string mount, KeyValue::Version version);

  std::string get(std::string path);
  std::string put(std::string path, std::unordered_map<std::string, std::string> map);
  std::string del(std::string path);
private:
  VaultClient client_;
  KeyValue::Version version_;
  std::string mount_;
  std::string getUrl(std::string path);
};
