#pragma once

#include <unordered_map>
#include <curl/curl.h>
#include <functional>
#include <experimental/optional>
#include <vector>

class AppRole;
class VaultClient;

struct HttpResponse {
  long statusCode;
  std::string body;
};

class AuthenticationStrategy {
public:
  virtual std::experimental::optional<std::string> authenticate(VaultClient* client) = 0;
};

using HttpErrorCallback = std::function<void(std::string)>;
using CurlSetupCallback = std::function<void(CURL *curl)>;

class VaultConfigBuilder;

class VaultConfig {
public:
  friend class VaultConfigBuilder;
  static VaultConfigBuilder make();
  bool getTls() { return tls_; }
  bool getDebug() { return debug_; }
  bool getVerify() { return verify_; }
  std::string getHost() { return host_; }
  std::string getPort() { return port_; }
  std::string getNamespace() { return ns_; }
private:
  VaultConfig() : tls_(true), debug_(false), host_("localhost"), port_("8200"), ns_("") {}
  bool tls_;
  bool debug_;
  bool verify_;
  std::string host_;
  std::string port_;
  std::string ns_;
};

class VaultConfigBuilder {
public:
  VaultConfigBuilder& tls(bool flag) {
    config_.tls_ = flag;
    return *this;
  }

  VaultConfigBuilder& debug(bool flag) {
    config_.debug_ = flag;
    return *this;
  }

  VaultConfigBuilder& verify(bool flag) {
    config_.verify_ = flag;
    return *this;
  }

  VaultConfigBuilder& host(std::string host) {
    config_.host_ = host;
    return *this;
  }

  VaultConfigBuilder& port(std::string port) {
    config_.port_ = port;
    return *this;
  }

  VaultConfigBuilder& ns(std::string ns) {
    config_.ns_ = ns;
    return *this;
  }

  VaultConfig& getConfig() {
    return config_;
  }

  operator VaultConfig&&() {
    return std::move(config_);
  }
private:
  VaultConfig config_;
};

class HttpClient {
public:
  HttpClient(VaultConfig& config);
  HttpClient(VaultConfig& config, HttpErrorCallback errorCallback);

  std::experimental::optional<HttpResponse> get(std::string url, std::string string, std::string ns);
  std::experimental::optional<HttpResponse> post(std::string url, std::string token, std::string ns, std::string value);
  std::experimental::optional<HttpResponse> del(std::string url, std::string token, std::string ns);
  std::experimental::optional<HttpResponse> list(std::string url, std::string token, std::string ns);

  static bool is_success(std::experimental::optional<HttpResponse> response);
private:
  bool debug_;
  bool verify_;
  HttpErrorCallback errorCallback_;
  std::experimental::optional<HttpResponse> executeRequest(std::string url, std::string token, std::string ns, CurlSetupCallback callback);
};

class VaultClient {
public:
  VaultClient(VaultConfig& config, AuthenticationStrategy& authStrategy);
  VaultClient(VaultConfig& config, AuthenticationStrategy& authStrategy, HttpErrorCallback httpErrorCallback);

  bool isAuthenticated() { return !token_.empty(); }

  std::string getToken() { return token_; }
  std::string getNamespace() { return namespace_; }
  std::string getUrl(std::string base, std::string path);

  HttpClient& getHttpClient() { return httpClient_; }
private:
  bool debug_;
  bool tls_;

  std::string host_;
  std::string port_;
  std::string token_;
  std::string namespace_;

  HttpClient httpClient_;
  AuthenticationStrategy& authStrategy_;
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
