#pragma once

#include <unordered_map>
#include <curl/curl.h>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace Vault {
  struct TinyString {
    TinyString() = default;

    explicit TinyString(std::string value) : value_(std::move(value)) {}

    friend std::ostream &operator<<(std::ostream &os, const TinyString &object) { return os << object.value(); }
    friend std::string operator+(const std::string &string, const TinyString &tiny) { return string + tiny.value(); }
    friend std::string operator+(const TinyString &tiny, const std::string &string) { return tiny.value() + string; }
    friend std::string operator+(const char *string, const TinyString &tiny) { return string + tiny.value(); }
    friend std::string operator+(const TinyString &tiny, const char *string) { return tiny.value() + string; }
    friend std::string operator+(const TinyString &tiny, const TinyString &other) { return tiny.value() + tiny.value(); }

    [[nodiscard]] bool empty() const { return value_.empty(); }
    [[nodiscard]] const char *c_str() const { return value_.c_str(); }
    [[nodiscard]] const std::string &value() const { return value_; }

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
  typedef TinyString Host;
  typedef TinyString Port;
  typedef TinyString Algorithm;
  typedef TinyString SecretMount;
  typedef TinyString RootCertificateType;

  struct TinyLong {
    TinyLong() = default;
    explicit TinyLong(const long value) : value_(value) {}

    friend long operator+(const long addend, const TinyLong other) { return addend + other.value_; }
    friend long operator+(const TinyLong other, const long addend) { return other.value_ + addend; }
    friend long operator+(const TinyLong current, const TinyLong other) { return current.value_ + other.value_; }

    friend std::string operator+(const char *other, const TinyLong current) { return other + std::to_string(current.value_); }

    [[nodiscard]] long value() const { return value_; }

  protected:
    long value_;
  };

  typedef TinyLong HttpResponseStatusCode;
  typedef TinyLong ConnectTimeout;
  typedef TinyLong TTL;

  namespace Algorithms {
    const static Vault::Algorithm SHA1 = Vault::Algorithm{"sha1"};
    const static Vault::Algorithm SHA2_224 = Vault::Algorithm{"sha2-224"};
    const static Vault::Algorithm SHA2_256 = Vault::Algorithm{"sha2-256"};
    const static Vault::Algorithm SHA2_384 = Vault::Algorithm{"sha2-384"};
    const static Vault::Algorithm SHA2_512 = Vault::Algorithm{"sha2-512"};
  }

  namespace RootCertificateTypes {
    const static Vault::RootCertificateType INTERNAL = Vault::RootCertificateType("internal");
    const static Vault::RootCertificateType EXPORTED = Vault::RootCertificateType("exported");
  }

  struct HttpResponse {
    HttpResponseStatusCode statusCode{};
    HttpResponseBodyString body;
  };

  struct AuthenticationResponse {
    HttpResponseBodyString rawResponse;
    Token token;
  };

  class ConfigBuilder;
  class Config;
  class Client;

  using Parameters = std::unordered_map<std::string, std::string>;
  using HttpErrorCallback = std::function<void(std::string)>;
  using CurlSetupCallback = std::function<void(CURL *curl)>;
  using CurlHeaderCallback = std::function<curl_slist*(curl_slist *chunk)>;
  using JsonProducer = std::function<std::string(const Parameters &parameters)>;
  using NoArgJsonProducer = std::function<std::string()>;

  class Base64 {
  public:
    static bool is_base64(unsigned char c);
    static std::string encode(const std::string& value);
    static std::string encode(unsigned char const *bytes_to_encode, unsigned int in_len);
    static std::string decode(std::string const &encoded_string);
  };

  class HttpClient {
  public:
    explicit HttpClient(Config &config);

    HttpClient(Config &config, HttpErrorCallback errorCallback);

    [[nodiscard]] virtual std::optional<HttpResponse> get(const Url &url, const Token &token, const Namespace &ns) const;
    [[nodiscard]] virtual std::optional<HttpResponse> post(const Url &url, const Token &token, const Namespace &ns, const std::string& value) const;
    [[nodiscard]] virtual std::optional<HttpResponse> post(const Url &url, const Token &token, const Namespace &ns, const std::string& value, const CurlHeaderCallback& headerCallback) const;
    [[nodiscard]] virtual std::optional<HttpResponse> put(const Url &url, const Token &token, const Namespace &ns, const std::string& value) const;
    [[nodiscard]] virtual std::optional<HttpResponse> del(const Url &url, const Token &token, const Namespace &ns) const;
    [[nodiscard]] virtual std::optional<HttpResponse> list(const Url &url, const Token &token, const Namespace &ns) const;

    static bool is_success(std::optional<HttpResponse> response);

  private:
    bool debug_;
    bool verify_;
    long connectTimeout_;
    HttpErrorCallback errorCallback_;

    [[nodiscard]] std::optional<HttpResponse> executeRequest(const Url &url, const Token &token, const Namespace &ns, const CurlSetupCallback &callback, const CurlHeaderCallback& headerCallback) const;
  };

  class Config {
  public:
    friend class ConfigBuilder;

    bool getTls() { return tls_; }
    bool getDebug() { return debug_; }
    bool getVerify() { return verify_; }
    ConnectTimeout getConnectTimeout() { return connectTimeout_; }
    Host getHost() { return host_; }
    Port getPort() { return port_; }
    Namespace getNamespace() { return ns_; }

  private:
    Config()
      : tls_(true)
      , debug_(false)
      , verify_(true)
      , connectTimeout_(ConnectTimeout{10})
      , host_(Host{"localhost"})
      , port_(Port{"8200"})
      , ns_("")
      {}

    bool tls_;
    bool debug_;
    bool verify_;
    ConnectTimeout connectTimeout_;
    Host host_;
    Port port_;
    Namespace ns_;
  };

  class ConfigBuilder {
  public:
    explicit operator Config &&() {
      return std::move(config_);
    }

    ConfigBuilder &withTlsEnabled(bool flag);
    ConfigBuilder &withDebug(bool flag);
    ConfigBuilder &withTlsVerification(bool flag);
    ConfigBuilder &withHost(Host host);
    ConfigBuilder &withPort(Port port);
    ConfigBuilder &withNamespace(Namespace ns);
    ConfigBuilder &withConnectTimeout(ConnectTimeout timeout);
    Config &build();

  private:
    Config config_;
  };

  class AuthenticationStrategy {
  public:
    virtual std::optional<AuthenticationResponse> authenticate(const Client &client) = 0;
  };

  class Client {
  public:
    Client(const Client &other, Token token);
    Client(Config &config, AuthenticationStrategy &authStrategy);
    Client(Config &config, AuthenticationStrategy &authStrategy, HttpErrorCallback httpErrorCallback);

    [[nodiscard]] virtual bool is_authenticated() const { return !token_.empty(); }
    [[nodiscard]] virtual Url getUrl(const std::string &base, const Path &path) const;
    [[nodiscard]] virtual bool getDebug() const { return debug_; }
    [[nodiscard]] virtual bool getTls() const { return tls_; }
    [[nodiscard]] virtual Host getHost() const { return host_; }
    [[nodiscard]] virtual Port getPort() const { return port_; }
    [[nodiscard]] virtual Token getToken() const { return token_; }
    [[nodiscard]] virtual Namespace getNamespace() const { return namespace_; }
    [[nodiscard]] virtual const HttpClient &getHttpClient() const { return httpClient_; }
    [[nodiscard]] virtual AuthenticationStrategy &getAuthenticationStrategy() const { return authStrategy_; }

  private:
    bool debug_;
    bool tls_;
    Host host_;
    Port port_;
    Token token_;
    Namespace namespace_;
    HttpClient httpClient_;
    AuthenticationStrategy &authStrategy_;
  };

  class HttpConsumer {
  public:
    static std::optional<std::string> get(const Client &client, const Url &url);
    static std::optional<std::string> list(const Client &client, const Url &url);
    static std::optional<std::string> post(const Client &client, const Url &url, const Parameters& parameters);
    static std::optional<std::string> post(const Client &client, const Url &url, const Parameters &parameters, const JsonProducer &jsonProducer);
    static std::optional<std::string> post(const Client &client, const Url &url, const Parameters &parameters, const CurlHeaderCallback &headerCallback);
    static std::optional<std::string> put(const Client &client, const Url &url, const Parameters &parameters, const JsonProducer &jsonProducer);
    static std::optional<std::string> del(const Client &client, const Url &url);
    static std::optional<Vault::AuthenticationResponse> authenticate(const Client &client, const Url &url, const NoArgJsonProducer &jsonProducer);
  };

  class TokenStrategy : public AuthenticationStrategy {
  public:
    explicit TokenStrategy(Token token) : token_(std::move(token)) {}

    std::optional<AuthenticationResponse> authenticate(const Client &vaultClient) override {
      return AuthenticationResponse{HttpResponseBodyString{""}, token_};
    }

  private:
    Vault::Token token_;
  };

  class AppRoleStrategy : public AuthenticationStrategy {
  public:
    AppRoleStrategy(RoleId roleId, SecretId secretId);

    std::optional<AuthenticationResponse> authenticate(const Client &client) override;

  private:
    static Url getUrl(const Client &vaultClient, const Path &path);

    RoleId roleId_;
    SecretId secretId_;
  };

  class WrappedSecretAppRoleStrategy : public AuthenticationStrategy {
  public:
    WrappedSecretAppRoleStrategy(RoleId role_id, const Token &token);

    std::optional<AuthenticationResponse> authenticate(const Client &client) override;

  private:
    RoleId roleId_;
    const Token &token_;
  };

  class LdapStrategy : public AuthenticationStrategy {
  public:
    LdapStrategy(std::string username, std::string password);

    std::optional<AuthenticationResponse> authenticate(const Client &client) override;

  private:
    static Url getUrl(const Client &client, const Path &username);

    std::string username_;
    std::string password_;
  };

  class KeyValue {
  public:
    enum Version {
      v1, v2
    };

    explicit KeyValue(const Client &client);

    KeyValue(const Client &client, Vault::SecretMount mount);
    KeyValue(const Client &client, KeyValue::Version version);
    KeyValue(const Client &client, Vault::SecretMount mount, KeyValue::Version version);

    std::optional<std::string> list(const Path &path);
    std::optional<std::string> read(const Path &path);
    std::optional<std::string> create(const Path &path, const Parameters &parameters);
    std::optional<std::string> update(const Path &path, const Parameters &parameters);
    std::optional<std::string> del(const Path &path);
    std::optional<std::string> del(const Path &path, std::vector<long> versions);
    std::optional<std::string> undelete(const Path& path, std::vector<long> versions);
    std::optional<std::string> destroy(const Path &path, std::vector<long> versions);
    std::optional<std::string> readConfig();
    std::optional<std::string> updateConfig(const Parameters& parameters);
    std::optional<std::string> readMetadata(const Path& path);
    std::optional<std::string> updateMetadata(const Path& path, const Parameters& parameters);
    std::optional<std::string> deleteMetadata(const Path& path);

  private:
    Url getUrl(const Path& path);
    Url getMetadataUrl(const Path& path);

    const Client &client_;
    KeyValue::Version version_;
    Vault::SecretMount mount_;
  };

  class Transit {
  public:
    explicit Transit(const Client &client);

    std::optional<std::string> encrypt(const Path &path, const Parameters &parameters);
    std::optional<std::string> decrypt(const Path &path, const Parameters &parameters);
    std::optional<std::string> generate_data_key(const Path &path, const Parameters &parameters);
    std::optional<std::string> generate_wrapped_data_key(const Path &path, const Parameters &parameters);
    std::optional<std::string> generate_random_bytes(int num_bytes, const Parameters &parameters);
    std::optional<std::string> hash(const Algorithm &algorithm, const Parameters &parameters);
    std::optional<std::string> hmac(const Path &key, const Algorithm &algorithm, const Parameters &Parameters);
    std::optional<std::string> sign(const Path &key, const Algorithm &algorithm, const Parameters &Parameters);
    std::optional<std::string> verify(const Path &key, const Algorithm &algorithm, const Parameters &Parameters);

  private:
    Url getUrl(const Path &path);

    const Client &client_;
  };

  class AppRole {
  public:
    explicit AppRole(const Client &client);

    std::optional<std::string> list();
    std::optional<std::string> create(const Path &roleName, const Parameters &parameters);
    std::optional<std::string> update(const Path &roleName, const Parameters &parameters);
    std::optional<std::string> read(const Path &roleName);
    std::optional<std::string> del(const Path &roleName);
    std::optional<std::string> getRoleId(const Path &roleName);
    std::optional<std::string> updateRoleId(const Path &roleName, const Parameters &parameters);
    std::optional<std::string> getRoleProperty(const Path &roleName, const Path &propertyName);
    std::optional<std::string> updateRoleProperty(const Path &roleName, const Path &propertyName, const Parameters &parameters);
    std::optional<std::string> deleteRoleProperty(const Path &roleName, const Path &propertyName);

    std::optional<std::string> generateSecretId(const Path &roleName, const Parameters &parameters);
    std::optional<std::string> listSecretAccessors(const Path &roleName);
    std::optional<std::string> readSecretId(const Path &roleName, const Parameters &parameters);
    std::optional<std::string> destroySecretId(const Path &roleName, const Parameters &parameters);
    std::optional<std::string> readSecretIdAccessor(const Path &roleName, const Parameters &parameters);
    std::optional<std::string> destroySecretIdAccessor(const Path &roleName, const Parameters &parameters);
    std::optional<std::string> customSecretId(const Path &roleName, const Parameters &parameters);
    std::optional<std::string> tidy(const Path &roleName);

  private:
    Vault::Url getUrl(const Path &path);

    const Client &client_;
  };

  class Sys {
  public:
    explicit Sys(const Client &client);

    std::optional<std::string> leader();
    std::optional<std::string> health();
    std::optional<std::string> health(const Url &leader);
    std::optional<std::string> wrap(const Parameters &parameters, const TTL &ttl);
    std::optional<std::string> unwrap(const Token &token);
    std::optional<std::string> lookup(const Token &token);
    std::optional<std::string> rewrap(const Token &token);

    class Auth {
    public:
      explicit Auth(const Client &client) : client_(client) {}

      std::optional<std::string> list();
      std::optional<std::string> enable(const Path &path, const Parameters &parameters);
      std::optional<std::string> disable(const Path &path);
      std::optional<std::string> readTuning(const Path &path);
      std::optional<std::string> tune(const Path &path, const Parameters &parameters);

    private:
      Url getUrl(const Path &path);

      const Client &client_;
    };

  private:
    Url getUrl(const Path &path);

    const Client &client_;
  };

  class Totp {
  public:
    explicit Totp(const Client &client) : client_(client) {}

    std::optional<std::string> create(const Path &path, const Parameters &parameters);
    std::optional<std::string> read(const Path &path);
    std::optional<std::string> list();
    std::optional<std::string> del(const Path &path);
    std::optional<std::string> generate(const Path &path);
    std::optional<std::string> validate(const Path &path, const Parameters &parameters);

  private:
    Url getUrl(const Path &path);

    const Client &client_;
  };

  class Cubbyhole {
  public:
    explicit Cubbyhole(const Client &client) : client_(client) {}

    std::optional<std::string> create(const Path &path, const Parameters &parameters);
    std::optional<std::string> read(const Path &path);
    std::optional<std::string> list(const Path &path);
    std::optional<std::string> del(const Path &path);

  private:
    Url getUrl(const Path &path);

    const Client &client_;
  };

  class Pki {
  public:
    explicit Pki(const Client &client) : client_(client) {}

    std::optional<std::string> generateRoot(const RootCertificateType &rootCertificateType, const Parameters &parameters);
    std::optional<std::string> setUrls(const Parameters &parameters);
    std::optional<std::string> createRole(const Path &path, const Parameters &parameters);
    std::optional<std::string> updateRole(const Path &path, const Parameters &parameters);
    std::optional<std::string> issue(const Path &path, const Parameters &parameters);
    std::optional<std::string> listCertificates();
    std::optional<std::string> readCertificate(const Path &path);
    std::optional<std::string> readCrlConfiguration();
    std::optional<std::string> setCrlConfiguration(const Parameters &parameters);
    std::optional<std::string> rotateCrl();
    std::optional<std::string> tidy(const Parameters &parameters);

  private:
    Url getUrl(const Path &path);

    const Client &client_;
  };
}