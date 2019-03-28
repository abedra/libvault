#pragma once

#include <unordered_map>
#include <curl/curl.h>
#include <functional>

class AppRole;

class HttpClient {
public:
  HttpClient();
  HttpClient(bool debug);

  std::string get(std::string url, std::string string);
  std::string post(std::string url, std::string token, std::string value);
  std::string del(std::string url, std::string token);
private:
  bool debug_;
  std::pair<CURLcode, std::string> executeRequest(std::string url, std::string token, std::function<void(CURL *curl)> callback);
};

class VaultClient {
public:
  VaultClient(std::string host, std::string port, AppRole& appRole);
  VaultClient(std::string host, std::string port, AppRole& appRole, bool debug);

  std::string getToken() { return token_; }
  HttpClient& getHttpClient() { return httpClient_; }

  std::string getUrl(std::string base, std::string path);
private:
  AppRole& appRole_;
  std::string host_;
  std::string port_;
  std::string token_;
  bool debug_;
  HttpClient httpClient_ = (bool)nullptr;
};

class AppRole {
public:
  AppRole(std::string role_id, std::string secret_id);

  std::string authenticate(VaultClient* vaultClient);
private:
  std::string role_id_;
  std::string secret_id_;

  std::string getUrl(VaultClient* vaultClient, std::string path);
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
