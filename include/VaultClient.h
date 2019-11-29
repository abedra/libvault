#pragma once

#include <unordered_map>
#include <curl/curl.h>
#include <functional>
#include <experimental/optional>
#include <utility>
#include <vector>

/* Start Tiny Types */
/* TODO
 *
 * Create String type and long type base that has typical operators
 *
*/

struct TinyString {
  explicit TinyString(std::string value) : value_(std::move(value)) { }

  friend std::ostream& operator<<(std::ostream& os, const TinyString& object) { return os << object.value(); }
  friend std::string operator+(const std::string& string, const TinyString& tiny) { return string + tiny.value(); }
  friend std::string operator+(const TinyString& tiny, const std::string& string) { return tiny.value() + string; }
  friend std::string operator+(const char* string, const TinyString& tiny) { return string + tiny.value(); }
  friend std::string operator+(const TinyString& tiny, const char* string) { return tiny.value() + string; }

  bool empty() const {
    return value_.empty();
  }

  const char * c_str() const {
    return value_.c_str();
  }

  const std::string& value() const {
    return value_;
  }

protected:
  std::string value_;
};

struct HttpResponseStatusCode {
  long value;
};

struct VaultConnectTimeout {
  long value;
};

typedef TinyString SecretId;
typedef TinyString HttpResponseBodyString;
typedef TinyString Url;
typedef TinyString Path;

struct Token {
  std::string value;
};

struct Namespace {
  std::string value;
};

struct RoleId {
  std::string value;
};

struct VaultHost {
  std::string value;
};

struct VaultPort {
  std::string value;
};

/* End Tiny Types */

/* Start Response Types */

struct HttpResponse {
  HttpResponseStatusCode statusCode{};
  HttpResponseBodyString body;
};

struct AuthenticationResponse {
  HttpResponseBodyString rawResponse;
  Token token;
};

/* End Response Types */

/* Start Forward Declarations */

class VaultConfigBuilder;
class VaultClient;
class VaultConfig;
class AuthenticationStrategy;

/* End Forward Declarations */

/* Start Callbacks */

using HttpErrorCallback = std::function<void(std::string)>;
using CurlSetupCallback = std::function<void(CURL *curl)>;

/* End Callbacks */

/* Start Aliases */

using Parameters = std::unordered_map<std::string, std::string>;
template <typename T> using optional = std::experimental::optional<T>;

/* End Aliases */

//TODO: kind of gross, do something else?
class Base64 {
public:
  static bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
  }

  static std::string encode(std::string value) {
    return encode(reinterpret_cast<const unsigned char*>(value.c_str()), value.length());
  }

  static std::string encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
      char_array_3[i++] = *(bytes_to_encode++);
      if (i == 3) {
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for(i = 0; (i <4) ; i++) {
          ret += base64_chars[char_array_4[i]];
        }

        i = 0;
      }
    }

    if (i) {
      for(j = i; j < 3; j++) {
        char_array_3[j] = '\0';
      }

      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (j = 0; (j < i + 1); j++) {
        ret += base64_chars[char_array_4[j]];
      }

      while((i++ < 3)) {
        ret += '=';
      }
    }

    return ret;
  }

  static std::string decode(std::string const& encoded_string) {
    std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
      char_array_4[i++] = encoded_string[in_]; in_++;
      if (i ==4) {
        for (i = 0; i <4; i++) {
          char_array_4[i] = base64_chars.find(char_array_4[i]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (i = 0; (i < 3); i++) {
          ret += char_array_3[i];
        }

        i = 0;
      }
    }

    if (i) {
      for (j = i; j <4; j++) {
        char_array_4[j] = 0;
      }

      for (j = 0; j <4; j++) {
        char_array_4[j] = base64_chars.find(char_array_4[j]);
      }

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (j = 0; (j < i - 1); j++) {
        ret += char_array_3[j];
      }
    }

    return ret;
  }
};

class HttpClient {
public:
  explicit HttpClient(VaultConfig& config);
  HttpClient(VaultConfig& config, HttpErrorCallback errorCallback);

  optional<HttpResponse> get(const Url& url, const Token& token, const Namespace& ns) const;
  optional<HttpResponse> post(const Url& url, const Token& token, const Namespace& ns, std::string value) const;
  optional<HttpResponse> del(const Url& url, const Token& token, const Namespace& ns) const;
  optional<HttpResponse> list(const Url& url, const Token& token, const Namespace& ns) const;

  static bool is_success(optional<HttpResponse> response);
private:
  bool debug_;
  bool verify_;
  long connectTimeout_;
  HttpErrorCallback errorCallback_;
  optional<HttpResponse> executeRequest(const Url& url, const Token& token, const Namespace& ns, const CurlSetupCallback& callback) const;
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
  , ns_({""})
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
  VaultClient(VaultConfig& config, AuthenticationStrategy& authStrategy);
  VaultClient(VaultConfig& config, AuthenticationStrategy& authStrategy, HttpErrorCallback httpErrorCallback);

  const HttpClient& getHttpClient() const { return httpClient_; }

  bool is_authenticated() const { return !token_.value.empty(); }

  Token getToken() const { return token_; }
  Namespace getNamespace() const { return namespace_; }
  Url getUrl(const std::string& base, const Path& path) const;

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
  virtual optional<AuthenticationResponse> authenticate(const VaultClient& client) = 0;
};

class VaultHttpConsumer {
public:
  static optional<std::string> post(const VaultClient& client, const Url& url, Parameters parameters);
};

class TokenStrategy : public AuthenticationStrategy {
public:
  explicit TokenStrategy(Token token) : token_(std::move(token)) {}
  optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override {
    return AuthenticationResponse{HttpResponseBodyString{""}, token_};
  }
private:
  Token token_;
};

class AppRoleStrategy : public AuthenticationStrategy {
public:
  AppRoleStrategy(RoleId roleId, SecretId secretId);
  optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override;

private:
  static Url getUrl(const VaultClient& vaultClient, const Path& path);
  RoleId roleId_;
  SecretId secretId_;
};

class Unwrap {
public:
  static optional<SecretId> unwrap(const VaultClient &client, const Token& token);

private:
  static Url getUrl(const VaultClient& client, const Path& path);
};

class WrappedSecretAppRoleStrategy : public AuthenticationStrategy {
public:
  WrappedSecretAppRoleStrategy(RoleId role_id, const Token& token);
  optional<AuthenticationResponse> authenticate(const VaultClient& vaultClient) override;

private:
  RoleId roleId_;
  const Token& token_;
};

class KeyValue {
public:
  enum Version { v1, v2 };

  explicit KeyValue(const VaultClient& client);
  KeyValue(const VaultClient& client, std::string mount);
  KeyValue(const VaultClient& client, KeyValue::Version version);
  KeyValue(const VaultClient& client, std::string mount, KeyValue::Version version);

  optional<std::string> list(const Path& path);
  optional<std::string> get(const Path& path);
  optional<std::string> put(const Path& path, std::unordered_map<std::string, std::string> map);
  optional<std::string> del(const Path& path);
  optional<std::string> del(const Path& path, std::vector<long> versions);
  optional<std::string> destroy(const Path& path, std::vector<long> versions);
private:
  const VaultClient& client_;
  KeyValue::Version version_;
  std::string mount_;

  Url getUrl(const Path& path);
  Url getMetadataUrl(const Path& path);
};

class Transit {
public:
  explicit Transit(const VaultClient& client);

  optional<std::string> encrypt(const Path& path, const Parameters& parameters);
  optional<std::string> decrypt(const Path& path, const Parameters& parameters);
  optional<std::string> generate_data_key(const Path& path, const Parameters& parameters);
  optional<std::string> generate_wrapped_data_key(const Path& path, const Parameters& parameters);
  optional<std::string> generate_random_bytes(int num_bytes, const Parameters& parameters);
  optional<std::string> hash(const std::string& algorithm, const Parameters& parameters);
  optional<std::string> hmac(const std::string& key, const std::string& algorithm, const Parameters& Parameters);
  optional<std::string> sign(const std::string& key, const std::string& algorithm, const Parameters& Parameters);
  optional<std::string> verify(const std::string& key, const std::string& algorithm, const Parameters& Parameters);
private:
  const VaultClient& client_;
  Url getUrl(const Path& path);
};
