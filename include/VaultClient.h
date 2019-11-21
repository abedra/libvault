#pragma once

#include <unordered_map>
#include <curl/curl.h>
#include <functional>
#include <experimental/optional>
#include <utility>
#include <vector>

class VaultConfigBuilder;
class AppRole;
class VaultClient;
class VaultConfig;
class AuthenticationStrategy;

using HttpErrorCallback = std::function<void(std::string)>;
using CurlSetupCallback = std::function<void(CURL *curl)>;
using Parameters = std::unordered_map<std::string, std::string>;
template <typename T> using optional = std::experimental::optional<T>;

struct HttpResponse {
  long statusCode;
  std::string body;
};

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

  optional<HttpResponse> get(const std::string& url, const std::string& string, const std::string& ns) const;
  optional<HttpResponse> post(const std::string& url, const std::string& token, const std::string& ns, std::string value) const;
  optional<HttpResponse> del(const std::string& url, const std::string& token, const std::string& ns) const;
  optional<HttpResponse> list(const std::string& url, const std::string& token, const std::string& ns) const;

  static bool is_success(optional<HttpResponse> response);
private:
  bool debug_;
  bool verify_;
  long connectTimeout_;
  HttpErrorCallback errorCallback_;
  optional<HttpResponse> executeRequest(const std::string& url, const std::string& token, const std::string& ns, const CurlSetupCallback& callback) const;
};

class VaultConfig {
public:
  friend class VaultConfigBuilder;
  bool getTls() { return tls_; }
  bool getDebug() { return debug_; }
  bool getVerify() { return verify_; }
  long getConnectTimeout() { return connectTimeout_; }
  std::string getHost() { return host_; }
  std::string getPort() { return port_; }
  std::string getNamespace() { return ns_; }
private:
  VaultConfig() : tls_(true), debug_(false), verify_(true), connectTimeout_(10), host_("localhost"), port_("8200"), ns_("") {}
  bool tls_;
  bool debug_;
  bool verify_;
  long connectTimeout_;
  std::string host_;
  std::string port_;
  std::string ns_;
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

  VaultConfigBuilder& withHost(std::string host) {
    config_.host_ = std::move(host);
    return *this;
  }

  VaultConfigBuilder& withPort(std::string port) {
    config_.port_ = std::move(port);
    return *this;
  }

  VaultConfigBuilder& withNamespace(std::string ns) {
    config_.ns_ = std::move(ns);
    return *this;
  }

  VaultConfigBuilder& withConnectTimeout(long timeout) {
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

  bool is_authenticated() const { return !token_.empty(); }

  std::string getToken() const { return token_; }
  std::string getNamespace() const { return namespace_; }
  std::string getUrl(const std::string& base, const std::string& path) const;

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

class AuthenticationStrategy {
public:
  virtual optional<std::string> authenticate(const VaultClient& client) = 0;
};

class VaultHttpConsumer {
public:
  static optional<std::string> post(const VaultClient& client, const std::string& uri, Parameters parameters);
};

class Token : public AuthenticationStrategy {
public:
  Token(std::string token) : token_(token) {}
  optional<std::string> authenticate(const VaultClient& vaultClient) {
    return token_;
  }
private:
  std::string token_;
};

class AppRole : public AuthenticationStrategy {
public:
  AppRole(std::string role_id, std::string secret_id);

  optional<std::string> authenticate(const VaultClient& vaultClient) override;
  static optional<AppRole> authenticateWrapped(const VaultClient& client, std::string token, std::string role_id)
private:
  std::string role_id_;
  std::string secret_id_;

  static std::string getUrl(const VaultClient& vaultClient, const std::string& path);
};

class Unwrap {
public:
    static optional<std::string> unwrap(const VaultClient &client, std::string token);

private:
    static std::string getUrl(const VaultClient& client, std::string path);
};

class KeyValue {
public:
  enum Version { v1, v2 };

  KeyValue(const VaultClient& client);
  KeyValue(const VaultClient& client, std::string mount);
  KeyValue(const VaultClient& client, KeyValue::Version version);
  KeyValue(const VaultClient& client, std::string mount, KeyValue::Version version);

  optional<std::string> list(std::string path);
  optional<std::string> get(std::string path);
  optional<std::string> put(std::string path, std::unordered_map<std::string, std::string> map);
  optional<std::string> del(std::string path);
  optional<std::string> del(std::string path, std::vector<long> versions);
  optional<std::string> destroy(std::string path, std::vector<long> versions);
private:
  const VaultClient& client_;
  KeyValue::Version version_;
  std::string mount_;

  std::string getUrl(std::string path);
  std::string getMetadataUrl(std::string path);
};

class Transit {
public:
  Transit(const VaultClient& client);

  optional<std::string> encrypt(std::string path, Parameters parameters);
  optional<std::string> decrypt(std::string path, Parameters parameters);
  optional<std::string> generate_data_key(std::string path, Parameters parameters);
  optional<std::string> generate_wrapped_data_key(std::string path, Parameters parameters);
  optional<std::string> generate_random_bytes(int num_bytes, Parameters parameters);
  optional<std::string> hash(std::string algorithm, Parameters parameters);
  optional<std::string> hmac(std::string key, std::string algorithm, Parameters Parameters);
  optional<std::string> sign(std::string key, std::string algorithm, Parameters Parameters);
  optional<std::string> verify(std::string key, std::string algorithm, Parameters Parameters);
private:
  const VaultClient& client_;
  std::string getUrl(std::string path);
};
