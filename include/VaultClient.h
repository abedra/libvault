#pragma once

#include <unordered_map>
#include <curl/curl.h>
#include <functional>
#include <experimental/optional>
#include <vector>

class AppRole;
class VaultClient;

struct CurlResponse {
  CURLcode curlCode;
  long statusCode;
  std::string body;
};

struct HttpResponse {
  HttpResponse(CurlResponse curlResponse) : statusCode(curlResponse.statusCode), body(curlResponse.body) {}
  long statusCode;
  std::string body;
};

class AuthenticationStrategy {
public:
  virtual std::experimental::optional<std::string> authenticate(VaultClient* client) = 0;
};

class HttpClient {
public:
  HttpClient();
  HttpClient(bool debug);

  std::experimental::optional<HttpResponse> get(std::string url, std::string string, std::string ns);
  std::experimental::optional<HttpResponse> post(std::string url, std::string token, std::string ns, std::string value);
  std::experimental::optional<HttpResponse> del(std::string url, std::string token, std::string ns);
  std::experimental::optional<HttpResponse> list(std::string url, std::string token, std::string ns);
private:
  bool debug_;
  CurlResponse executeRequest(std::string url, std::string token, std::string ns, std::function<void(CURL *curl)> callback);
};

class VaultClient {
public:
  VaultClient(std::string host, std::string port, AuthenticationStrategy& authStrategy);
  VaultClient(std::string host, std::string port, AuthenticationStrategy& authStrategy, bool debug);

  void setNamespace(std::string ns) { namespace_ = ns; }

  std::string getToken() { return token_; }
  std::string getNamespace() { return namespace_; }
  std::string getUrl(std::string base, std::string path);
  HttpClient& getHttpClient() { return httpClient_; }

  bool isAuthenticated() { return !token_.empty(); }
private:
  AuthenticationStrategy& authStrategy_;
  std::string host_;
  std::string port_;
  std::string token_;
  std::string namespace_;
  bool debug_;
  HttpClient httpClient_ = (bool)nullptr;
};

class AppRole : public AuthenticationStrategy {
public:
  AppRole(std::string role_id, std::string secret_id);

  std::experimental::optional<std::string> authenticate(VaultClient* vaultClient);
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

  std::experimental::optional<std::string> list(std::string path);
  std::experimental::optional<std::string> get(std::string path);
  std::experimental::optional<std::string> put(std::string path, std::unordered_map<std::string, std::string> map);
  std::experimental::optional<std::string> del(std::string path);
  std::experimental::optional<std::string> del(std::string path, std::vector<long> versions);
  std::experimental::optional<std::string> destroy(std::string path, std::vector<long> versions);
private:
  VaultClient client_;
  KeyValue::Version version_;
  std::string mount_;

  std::string getUrl(std::string path);
  std::string getMetadataUrl(std::string path);
};
