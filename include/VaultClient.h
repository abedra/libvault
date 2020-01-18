#pragma once

#include <unordered_map>
#include <curl/curl.h>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include "json.hpp"

/* Tiny Types */

struct TinyString {
  TinyString() = default;
  explicit TinyString(std::string value) : value_(std::move(value)) { }

  friend std::ostream& operator<<(std::ostream& os, const TinyString& object) { return os << object.value(); }
  friend std::string operator+(const std::string& string, const TinyString& tiny) { return string + tiny.value(); }
  friend std::string operator+(const TinyString& tiny, const std::string& string) { return tiny.value() + string; }
  friend std::string operator+(const char* string, const TinyString& tiny) { return string + tiny.value(); }
  friend std::string operator+(const TinyString& tiny, const char* string) { return tiny.value() + string; }
  friend std::string operator+(const TinyString& tiny, const TinyString& other) { return tiny.value() + tiny.value(); }

  [[nodiscard]] bool empty() const { return value_.empty(); }
  [[nodiscard]] const char* c_str() const { return value_.c_str(); }
  [[nodiscard]] const std::string& value() const { return value_; }

protected:
  std::string value_;
};

namespace Vault {
  typedef TinyString SecretId;
  typedef TinyString HttpResponseBodyString;
  typedef TinyString Url;
  typedef TinyString Path;
  typedef TinyString Token;
  typedef TinyString Namespace;
  typedef TinyString RoleId;
  typedef TinyString Host;
  typedef TinyString Port;
  typedef TinyString Algorithm;
  typedef TinyString SecretMount;

  // TODO: Create tiny long type
  struct HttpResponseStatusCode {
    long value;
  };

  struct ConnectTimeout {
    long value;
  };

  namespace Algorithms {
    const static Vault::Algorithm SHA1 = Vault::Algorithm("sha1");
    const static Vault::Algorithm SHA2_224 = Vault::Algorithm{"sha2-224"};
    const static Vault::Algorithm SHA2_256 = Vault::Algorithm{"sha2-256"};
    const static Vault::Algorithm SHA2_384 = Vault::Algorithm{"sha2-384"};
    const static Vault::Algorithm SHA2_512 = Vault::Algorithm{"sha2-512"};
  };
}

/* Response Types */

struct HttpResponse {
  Vault::HttpResponseStatusCode statusCode{};
  Vault::HttpResponseBodyString body;
};

struct AuthenticationResponse {
  Vault::HttpResponseBodyString rawResponse;
  Vault::Token token;
};

/* Forward Declarations */

class VaultConfigBuilder;
class VaultConfig;
class VaultClient;

/* Aliases */

using Parameters = std::unordered_map<std::string, std::string>;

/* Callbacks */

using HttpErrorCallback = std::function<void(std::string)>;
using CurlSetupCallback = std::function<void(CURL *curl)>;
using JsonProducer = std::function<nlohmann::json(const Parameters& parameters)>;

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

  [[nodiscard]] virtual std::optional<HttpResponse> get(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns) const;
  [[nodiscard]] virtual std::optional<HttpResponse> post(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns, std::string value) const;
  [[nodiscard]] virtual std::optional<HttpResponse> del(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns) const;
  [[nodiscard]] virtual std::optional<HttpResponse> list(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns) const;

  static bool is_success(std::optional<HttpResponse> response);
private:
  bool debug_;
  bool verify_;
  long connectTimeout_;
  HttpErrorCallback errorCallback_;
  [[nodiscard]] std::optional<HttpResponse> executeRequest(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns, const CurlSetupCallback& callback) const;
};

class VaultConfig {
public:
  friend class VaultConfigBuilder;
  bool getTls() { return tls_; }
  bool getDebug() { return debug_; }
  bool getVerify() { return verify_; }
  Vault::ConnectTimeout getConnectTimeout() { return connectTimeout_; }
  Vault::Host getHost() { return host_; }
  Vault::Port getPort() { return port_; }
  Vault::Namespace getNamespace() { return ns_; }

private:
  VaultConfig()
  : tls_(true)
  , debug_(false)
  , verify_(true)
  , connectTimeout_(Vault::ConnectTimeout{10})
  , host_(Vault::Host{"localhost"})
  , port_(Vault::Port{"8200"})
  , ns_("")
  {}

  bool tls_;
  bool debug_;
  bool verify_;
  Vault::ConnectTimeout connectTimeout_;
  Vault::Host host_;
  Vault::Port port_;
  Vault::Namespace ns_;
};

class VaultConfigBuilder {
public:
  explicit operator VaultConfig&&() {
    return std::move(config_);
  }

  VaultConfigBuilder& withTlsEnabled(bool flag);
  VaultConfigBuilder& withDebug(bool flag);
  VaultConfigBuilder& withTlsVerification(bool flag);
  VaultConfigBuilder& withHost(Vault::Host host);
  VaultConfigBuilder& withPort(Vault::Port port);
  VaultConfigBuilder& withNamespace(Vault::Namespace ns);
  VaultConfigBuilder& withConnectTimeout(Vault::ConnectTimeout timeout);
  VaultConfig& build();

private:
  VaultConfig config_;
};

class AuthenticationStrategy {
public:
  virtual std::optional<AuthenticationResponse> authenticate(const VaultClient& client) = 0;
};

class VaultClient {
public:
  VaultClient(const VaultClient& other, const Vault::Token& token);
  VaultClient(VaultConfig& config, AuthenticationStrategy& authStrategy);
  VaultClient(VaultConfig& config, AuthenticationStrategy& authStrategy, HttpErrorCallback httpErrorCallback);

  [[nodiscard]] virtual bool is_authenticated() const { return !token_.empty(); }
  [[nodiscard]] virtual Vault::Url getUrl(const std::string& base, const Vault::Path& path) const;

  [[nodiscard]] virtual bool getDebug() const { return debug_; }
  [[nodiscard]] virtual bool getTls() const { return tls_; }
  [[nodiscard]] virtual Vault::Host getHost() const { return host_; }
  [[nodiscard]] virtual Vault::Port getPort() const { return port_; }
  [[nodiscard]] virtual Vault::Token getToken() const { return token_; }
  [[nodiscard]] virtual Vault::Namespace getNamespace() const { return namespace_; }
  [[nodiscard]] virtual const HttpClient& getHttpClient() const { return httpClient_; }
  [[nodiscard]] virtual AuthenticationStrategy& getAuthenticationStrategy() const { return authStrategy_; }

private:
  bool debug_;
  bool tls_;
  Vault::Host host_;
  Vault::Port port_;
  Vault::Token token_;
  Vault::Namespace namespace_;
  HttpClient httpClient_;
  AuthenticationStrategy& authStrategy_;
};

class VaultHttpConsumer {
public:
  static std::optional<std::string> get(const VaultClient& client, const Vault::Url& url);
  static std::optional<std::string> list(const VaultClient& client, const Vault::Url& url);
  static std::optional<std::string> post(const VaultClient& client, const Vault::Url& url, Parameters parameters);
  static std::optional<std::string> post(const VaultClient& client, const Vault::Url& url, const Parameters& parameters, const JsonProducer& jsonProducer);
  static std::optional<std::string> del(const VaultClient& client, const Vault::Url& url);
};

class Unwrap {
public:
  static std::optional<Vault::SecretId> unwrap(const VaultClient &client);

private:
  static Vault::Url getUrl(const VaultClient& client, const Vault::Path& path);
};

class TokenStrategy : public AuthenticationStrategy {
public:
  explicit TokenStrategy(Vault::Token token) : token_(std::move(token)) {}

  std::optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override {
    return AuthenticationResponse{Vault::HttpResponseBodyString{""}, token_};
  }

private:
  Vault::Token token_;
};

class AppRoleStrategy : public AuthenticationStrategy {
public:
  AppRoleStrategy(Vault::RoleId roleId, Vault::SecretId secretId);

  std::optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override;
private:
  static Vault::Url getUrl(const VaultClient& vaultClient, const Vault::Path& path);
  Vault::RoleId roleId_;
  Vault::SecretId secretId_;
};

class WrappedSecretAppRoleStrategy : public AuthenticationStrategy {
public:
  WrappedSecretAppRoleStrategy(Vault::RoleId role_id, const Vault::Token& token);

  std::optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override;

private:
  Vault::RoleId roleId_;
  const Vault::Token& token_;
};

class LdapStrategy : public AuthenticationStrategy {
public:
  LdapStrategy(std::string  username, std::string  password);

  std::optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override;

private:
  static Vault::Url getUrl(const VaultClient& client, const Vault::Path& username);

  std::string username_;
  std::string password_;
};

class KeyValue {
public:
  enum Version { v1, v2 };

  explicit KeyValue(const VaultClient& client);
  KeyValue(const VaultClient& client, Vault::SecretMount  mount);
  KeyValue(const VaultClient& client, KeyValue::Version version);
  KeyValue(const VaultClient& client, Vault::SecretMount mount, KeyValue::Version version);

  std::optional<std::string> list(const Vault::Path& path);
  std::optional<std::string> get(const Vault::Path& path);
  std::optional<std::string> put(const Vault::Path& path, const Parameters& parameters);
  std::optional<std::string> del(const Vault::Path& path);
  std::optional<std::string> del(const Vault::Path& path, std::vector<long> versions);
  std::optional<std::string> destroy(const Vault::Path& path, std::vector<long> versions);

private:
  Vault::Url getUrl(const Vault::Path& path);
  Vault::Url getMetadataUrl(const Vault::Path& path);

  const VaultClient& client_;
  KeyValue::Version version_;
  Vault::SecretMount mount_;
};

class Transit {
public:
  explicit Transit(const VaultClient& client);

  std::optional<std::string> encrypt(const Vault::Path& path, const Parameters& parameters);
  std::optional<std::string> decrypt(const Vault::Path& path, const Parameters& parameters);
  std::optional<std::string> generate_data_key(const Vault::Path& path, const Parameters& parameters);
  std::optional<std::string> generate_wrapped_data_key(const Vault::Path& path, const Parameters& parameters);
  std::optional<std::string> generate_random_bytes(int num_bytes, const Parameters& parameters);
  std::optional<std::string> hash(const Vault::Algorithm& algorithm, const Parameters& parameters);
  std::optional<std::string> hmac(const Vault::Path& key, const Vault::Algorithm& algorithm, const Parameters& Parameters);
  std::optional<std::string> sign(const Vault::Path& key, const Vault::Algorithm& algorithm, const Parameters& Parameters);
  std::optional<std::string> verify(const Vault::Path& key, const Vault::Algorithm& algorithm, const Parameters& Parameters);

private:
  Vault::Url getUrl(const Vault::Path& path);
  const VaultClient& client_;
};

class AppRole {
public:
  explicit AppRole(const VaultClient& client);

  std::optional<std::string> list();
  std::optional<std::string> create(const Vault::Path& roleName, const Parameters& parameters);
  std::optional<std::string> update(const Vault::Path& roleName, const Parameters& parameters);
  std::optional<std::string> read(const Vault::Path& roleName);

private:
  Vault::Url getUrl(const Vault::Path& path);
  const VaultClient& client_;
};

class Sys {
public:
  explicit Sys(const VaultClient& client);

  std::optional<std::string> leader();
  std::optional<std::string> health();
  std::optional<std::string> health(const Vault::Url& leader);
private:
  Vault::Url getUrl(const Vault::Path& path);
  const VaultClient& client_;
};