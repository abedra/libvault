#pragma once

#include <unordered_map>
#include <curl/curl.h>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

/* Tiny Types */

struct TinyString {
  TinyString() = default;
  explicit TinyString(std::string value) : value_(std::move(value)) { }

  friend std::ostream& operator<<(std::ostream& os, const TinyString& object) { return os << object.value(); }
  friend std::string operator+(const std::string& string, const TinyString& tiny) { return string + tiny.value(); }
  friend std::string operator+(const TinyString& tiny, const std::string& string) { return tiny.value() + string; }
  friend std::string operator+(const char* string, const TinyString& tiny) { return string + tiny.value(); }
  friend std::string operator+(const TinyString& tiny, const char* string) { return tiny.value() + string; }

  [[nodiscard]] bool empty() const {
    return value_.empty();
  }

  [[nodiscard]] const char* c_str() const {
    return value_.c_str();
  }

  [[nodiscard]] const std::string& value() const {
    return value_;
  }

protected:
  std::string value_;
};

typedef TinyString SecretId;
typedef TinyString HttpResponseBodyString;
typedef TinyString Url;
typedef TinyString Path;
typedef TinyString Token;
typedef TinyString Namespace;
typedef TinyString RoleId;
typedef TinyString VaultHost;
typedef TinyString VaultPort;
typedef TinyString Algorithm;
typedef TinyString SecretMount;

namespace Algorithms {
  const static Algorithm SHA1 = Algorithm("sha1");
  const static Algorithm SHA2_224 = Algorithm{"sha2-224"};
  const static Algorithm SHA2_256 = Algorithm{"sha2-256"};
  const static Algorithm SHA2_384 = Algorithm{"sha2-384"};
  const static Algorithm SHA2_512 = Algorithm{"sha2-512"};
};

// TODO: Create tiny long type
struct HttpResponseStatusCode {
  long value;
};

struct VaultConnectTimeout {
  long value;
};

/* Response Types */

struct HttpResponse {
  HttpResponseStatusCode statusCode{};
  HttpResponseBodyString body;
};

struct AuthenticationResponse {
  HttpResponseBodyString rawResponse;
  Token token;
};

/* Forward Declarations */

class VaultConfigBuilder;
class VaultClient;
class VaultConfig;
class AuthenticationStrategy;

/* Callbacks */

using HttpErrorCallback = std::function<void(std::string)>;
using CurlSetupCallback = std::function<void(CURL *curl)>;

/* Aliases */

using Parameters = std::unordered_map<std::string, std::string>;

/* Classes */

class Base64 {
public:
  static bool is_base64(unsigned char c);
  static std::string encode(std::string value);
  static std::string encode(unsigned char const* bytes_to_encode, unsigned int in_len);
  static std::string decode(std::string const& encoded_string);
};

class HttpClient {
public:
  explicit HttpClient(VaultConfig& config);
  HttpClient(VaultConfig& config, HttpErrorCallback errorCallback);

  [[nodiscard]] std::optional<HttpResponse> get(const Url& url, const Token& token, const Namespace& ns) const;
  [[nodiscard]] std::optional<HttpResponse> post(const Url& url, const Token& token, const Namespace& ns, std::string value) const;
  [[nodiscard]] std::optional<HttpResponse> del(const Url& url, const Token& token, const Namespace& ns) const;
  [[nodiscard]] std::optional<HttpResponse> list(const Url& url, const Token& token, const Namespace& ns) const;

  static bool is_success(std::optional<HttpResponse> response);
private:
  bool debug_;
  bool verify_;
  long connectTimeout_;
  HttpErrorCallback errorCallback_;
  [[nodiscard]] std::optional<HttpResponse> executeRequest(const Url& url, const Token& token, const Namespace& ns, const CurlSetupCallback& callback) const;
};

class VaultConfig {
public:
  friend class VaultConfigBuilder;
  bool getTls() { return tls_; }
  bool getDebug() { return debug_; }
  bool getVerify() { return verify_; }
  VaultConnectTimeout getConnectTimeout() { return connectTimeout_; }
  VaultHost getHost() { return host_; }
  VaultPort getPort() { return port_; }
  Namespace getNamespace() { return ns_; }

private:
  VaultConfig()
  : tls_(true)
  , debug_(false)
  , verify_(true)
  , connectTimeout_(VaultConnectTimeout{10})
  , host_(VaultHost{"localhost"})
  , port_(VaultPort{"8200"})
  , ns_("")
  {}

  bool tls_;
  bool debug_;
  bool verify_;
  VaultConnectTimeout connectTimeout_;
  VaultHost host_;
  VaultPort port_;
  Namespace ns_;
};

class VaultConfigBuilder {
public:
  VaultConfigBuilder& withTlsEnabled(bool flag) {
    config_.tls_ = flag;
    return *this;
  }

  VaultConfigBuilder& withDebug(bool flag) {
    config_.debug_ = flag;
    return *this;
  }

  VaultConfigBuilder& withTlsVerification(bool flag) {
    config_.verify_ = flag;
    return *this;
  }

  VaultConfigBuilder& withHost(VaultHost host) {
    config_.host_ = std::move(host);
    return *this;
  }

  VaultConfigBuilder& withPort(VaultPort port) {
    config_.port_ = std::move(port);
    return *this;
  }

  VaultConfigBuilder& withNamespace(Namespace ns) {
    config_.ns_ = std::move(ns);
    return *this;
  }

  VaultConfigBuilder& withConnectTimeout(VaultConnectTimeout timeout) {
      config_.connectTimeout_ = timeout;
      return *this;
  }

  VaultConfig& build() {
    return config_;
  }

  explicit operator VaultConfig&&() {
    return std::move(config_);
  }
private:
  VaultConfig config_;
};

class VaultClient {
public:
  VaultClient(const VaultClient& other, const Token& token);
  VaultClient(VaultConfig& config, AuthenticationStrategy& authStrategy);
  VaultClient(VaultConfig& config, AuthenticationStrategy& authStrategy, HttpErrorCallback httpErrorCallback);

  [[nodiscard]] bool is_authenticated() const { return !token_.empty(); }
  [[nodiscard]] Url getUrl(const std::string& base, const Path& path) const;

  [[nodiscard]] bool getDebug() const { return debug_; }
  [[nodiscard]] bool getTls() const { return tls_; }
  [[nodiscard]] VaultHost getHost() const { return host_; }
  [[nodiscard]] VaultPort getPort() const { return port_; }
  [[nodiscard]] Token getToken() const { return token_; }
  [[nodiscard]] Namespace getNamespace() const { return namespace_; }
  [[nodiscard]] const HttpClient& getHttpClient() const { return httpClient_; }
  [[nodiscard]] AuthenticationStrategy& getAuthenticationStrategy() const { return authStrategy_; }

private:
  bool debug_;
  bool tls_;
  VaultHost host_;
  VaultPort port_;
  Token token_;
  Namespace namespace_;
  HttpClient httpClient_;
  AuthenticationStrategy& authStrategy_;
};

class AuthenticationStrategy {
public:
  virtual std::optional<AuthenticationResponse> authenticate(const VaultClient& client) = 0;
};

class VaultHttpConsumer {
public:
  static std::optional<std::string> post(const VaultClient& client, const Url& url, Parameters parameters);
};

class TokenStrategy : public AuthenticationStrategy {
public:
  explicit TokenStrategy(Token token) : token_(std::move(token)) {}

  std::optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override {
    return AuthenticationResponse{HttpResponseBodyString{""}, token_};
  }

private:
  Token token_;
};

class AppRoleStrategy : public AuthenticationStrategy {
public:
  AppRoleStrategy(RoleId roleId, SecretId secretId);

  std::optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override;

private:
  static Url getUrl(const VaultClient& vaultClient, const Path& path);
  RoleId roleId_;
  SecretId secretId_;
};

class Unwrap {
public:
  static std::optional<SecretId> unwrap(const VaultClient &client);

private:
  static Url getUrl(const VaultClient& client, const Path& path);
};

class WrappedSecretAppRoleStrategy : public AuthenticationStrategy {
public:
  WrappedSecretAppRoleStrategy(RoleId role_id, const Token& token);

  std::optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override;

private:
  RoleId roleId_;
  const Token& token_;
};

class KeyValue {
public:
  enum Version { v1, v2 };

  explicit KeyValue(const VaultClient& client);
  KeyValue(const VaultClient& client, SecretMount  mount);
  KeyValue(const VaultClient& client, KeyValue::Version version);
  KeyValue(const VaultClient& client, SecretMount mount, KeyValue::Version version);

  std::optional<std::string> list(const Path& path);
  std::optional<std::string> get(const Path& path);
  std::optional<std::string> put(const Path& path, Parameters parameters);
  std::optional<std::string> del(const Path& path);
  std::optional<std::string> del(const Path& path, std::vector<long> versions);
  std::optional<std::string> destroy(const Path& path, std::vector<long> versions);

private:
  Url getUrl(const Path& path);
  Url getMetadataUrl(const Path& path);

  const VaultClient& client_;
  KeyValue::Version version_;
  SecretMount mount_;
};

class Transit {
public:
  explicit Transit(const VaultClient& client);

  std::optional<std::string> encrypt(const Path& path, const Parameters& parameters);
  std::optional<std::string> decrypt(const Path& path, const Parameters& parameters);
  std::optional<std::string> generate_data_key(const Path& path, const Parameters& parameters);
  std::optional<std::string> generate_wrapped_data_key(const Path& path, const Parameters& parameters);
  std::optional<std::string> generate_random_bytes(int num_bytes, const Parameters& parameters);
  std::optional<std::string> hash(const Algorithm& algorithm, const Parameters& parameters);
  std::optional<std::string> hmac(const Path& key, const Algorithm& algorithm, const Parameters& Parameters);
  std::optional<std::string> sign(const Path& key, const Algorithm& algorithm, const Parameters& Parameters);
  std::optional<std::string> verify(const Path& key, const Algorithm& algorithm, const Parameters& Parameters);

private:
  Url getUrl(const Path& path);

  const VaultClient& client_;
};
