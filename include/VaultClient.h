#pragma GCC diagnostic push
#pragma ide diagnostic ignored "UnusedStructInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma ide diagnostic ignored "UnusedGlobalDeclarationInspection"
#pragma once

#include <unordered_map>
#include <curl/curl.h>
#include <filesystem>
#include <functional>
#include <optional>
#include <ostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

namespace Vault {
  template<typename Name, typename T>
  struct Tiny {
    Tiny() noexcept(std::is_nothrow_constructible_v<T>) = default;
    explicit Tiny(const T &value) noexcept(std::is_nothrow_copy_constructible_v<T>) : value_(value) {}
    explicit Tiny(T &&value) noexcept(std::is_nothrow_move_constructible_v<T>) : value_(std::move(value)) {}

    friend std::ostream &operator<<(std::ostream &os, const Tiny &object) { return os << object.value(); }
    friend std::string operator+(const Tiny &tiny, const Tiny &other) { return tiny.value() + tiny.value(); }

    friend std::string operator+(const Tiny &tiny, const char *string) { return tiny.toString() + string; }
    friend std::string operator+(const char *string, const Tiny &tiny) { return string + tiny.toString(); }
    friend std::string operator+(const Tiny &tiny, const std::string &string) { return tiny.toString() + string; }
    friend std::string operator+(const std::string &string, const Tiny &tiny) { return string + tiny.toString(); }

    explicit operator T& () const noexcept {
      return value();
    }

    [[nodiscard]] std::string toString() const {
      std::ostringstream ss;
      ss << value();
      return ss.str();
    }

    [[nodiscard]] bool empty() const noexcept { return value_.empty(); }
    [[nodiscard]] const T &value() const noexcept { return value_; }

  protected:
    T value_;
  };

  #define LIBVAULT_TINY_STRING(Name)              \
    struct Name##Detail {};                       \
    using Name = Tiny<Name##Detail, std::string>; \

  #define LIBVAULT_TINY_LONG(Name)         \
    struct Name##Detail {};                \
    using Name = Tiny<Name##Detail, long>; \

  LIBVAULT_TINY_STRING(SecretId)
  LIBVAULT_TINY_STRING(HttpResponseBodyString)
  LIBVAULT_TINY_STRING(Url)
  LIBVAULT_TINY_STRING(Path)
  LIBVAULT_TINY_STRING(Token)
  LIBVAULT_TINY_STRING(Namespace)
  LIBVAULT_TINY_STRING(RoleId)
  LIBVAULT_TINY_STRING(Host)
  LIBVAULT_TINY_STRING(Port)
  LIBVAULT_TINY_STRING(Algorithm)
  LIBVAULT_TINY_STRING(SecretMount)
  LIBVAULT_TINY_STRING(RootCertificateType)
  LIBVAULT_TINY_STRING(KeyType)

  LIBVAULT_TINY_LONG(HttpResponseStatusCode)
  LIBVAULT_TINY_LONG(ConnectTimeout)
  LIBVAULT_TINY_LONG(TTL)

  #undef LIBVAULT_TINY_STRING
  #undef LIBVAULT_TINY_LONG

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

  namespace KeyTypes {
    const static Vault::KeyType ENCRYPTION_KEY = Vault::KeyType{"encryption-key"};
    const static Vault::KeyType SIGNING_KEY = Vault::KeyType{"signing-key"};
    const static Vault::KeyType HMAC_KEY = Vault::KeyType{"hmac-key"};
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
    virtual ~HttpClient() = default;

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
    std::filesystem::path caBundle_;
    HttpErrorCallback errorCallback_;

    [[nodiscard]] std::optional<HttpResponse> executeRequest(const Url &url, const Token &token, const Namespace &ns, const CurlSetupCallback &callback, const CurlHeaderCallback& headerCallback) const;
  };

  class Config {
  public:
    friend class ConfigBuilder;

    [[nodiscard]] bool getTls() const { return tls_; }
    [[nodiscard]] bool getDebug() const { return debug_; }
    [[nodiscard]] bool getVerify() const { return verify_; }
    ConnectTimeout getConnectTimeout() { return connectTimeout_; }
    Host getHost() { return host_; }
    Port getPort() { return port_; }
    Namespace getNamespace() { return ns_; }
    std::filesystem::path getCaBundle() { return caBundle_; }

  private:
    Config()
      : tls_(true)
      , debug_(false)
      , verify_(true)
      , connectTimeout_(ConnectTimeout{10})
      , host_(Host{"localhost"})
      , port_(Port{"8200"})
      , ns_("")
      , caBundle_()
      {}

    bool tls_;
    bool debug_;
    bool verify_;
    ConnectTimeout connectTimeout_;
    Host host_;
    Port port_;
    Namespace ns_;
    std::filesystem::path caBundle_;
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
    ConfigBuilder &withCaBundle(const std::filesystem::path &caBundle);
    Config &build();

  private:
    Config config_;
  };

  class AuthenticationStrategy {
  public:
    virtual ~AuthenticationStrategy() = default;
    virtual std::optional<AuthenticationResponse> authenticate(const Client &client) = 0;
  };

  class Client {
  public:
    Client(const Client &other, Token token);
    Client(Config &config, AuthenticationStrategy &authStrategy);
    Client(Config &config, AuthenticationStrategy &authStrategy, HttpErrorCallback httpErrorCallback);
    virtual ~Client() = default;

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
    [[nodiscard]] virtual std::filesystem::path getCaBundle() const { return caBundle_; }

  private:
    bool debug_;
    bool tls_;
    Host host_;
    Port port_;
    Token token_;
    Namespace namespace_;
    HttpClient httpClient_;
    AuthenticationStrategy &authStrategy_;
    std::filesystem::path caBundle_;
  };

  class HttpConsumer {
  public:
    static std::optional<std::string> get(const Client &client, const Url &url);
    static std::optional<std::string> list(const Client &client, const Url &url);
    static std::optional<std::string> post(const Client &client, const Url &url, const Parameters& parameters);
    static std::optional<std::string> post(const Client &client, const Url &url, const Parameters &parameters, const JsonProducer &jsonProducer);
    static std::optional<std::string> post(const Client &client, const Url &url, const Parameters &parameters, const CurlHeaderCallback &headerCallback);
    static std::optional<std::string> put(const Client &client, const Url &url, const Parameters &parameters);
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
    explicit Transit(const Client &client) : client_(client) {}

    std::optional<std::string> createKey(const Path &path, const Parameters &parameters);
    std::optional<std::string> readKey(const Path &path);
    std::optional<std::string> deleteKey(const Path &path);
    std::optional<std::string> listKeys();
    std::optional<std::string> updateKeyConfiguration(const Path &path, const Parameters &parameters);
    std::optional<std::string> rotateKey(const Path &path);
    std::optional<std::string> exportKey(const KeyType &keyType, const Path &path, const Path &version);
    std::optional<std::string> encrypt(const Path &path, const Parameters &parameters);
    std::optional<std::string> decrypt(const Path &path, const Parameters &parameters);
    std::optional<std::string> rewrap(const Path &path, const Parameters &parameters);
    std::optional<std::string> generateDataKey(const Path &path, const Parameters &parameters);
    std::optional<std::string> generateWrappedDataKey(const Path &path, const Parameters &parameters);
    std::optional<std::string> generateRandomBytes(int num_bytes, const Parameters &parameters);
    std::optional<std::string> hash(const Algorithm &algorithm, const Parameters &parameters);
    std::optional<std::string> hmac(const Path &key, const Algorithm &algorithm, const Parameters &Parameters);
    std::optional<std::string> sign(const Path &key, const Algorithm &algorithm, const Parameters &Parameters);
    std::optional<std::string> verify(const Path &key, const Algorithm &algorithm, const Parameters &Parameters);
    std::optional<std::string> backup(const Path &path);
    std::optional<std::string> restore(const Path &path, const Parameters &parameters);
    std::optional<std::string> trim(const Path &path, const Parameters &parameters);
    std::optional<std::string> configureCache(const Parameters &parameters);
    std::optional<std::string> readCacheConfiguration();

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
    explicit Sys(const Client &client) : client_(client) {}

    std::optional<std::string> leader();
    std::optional<std::string> health();
    std::optional<std::string> health(const Url &leader);
    std::optional<std::string> wrap(const Parameters &parameters, const TTL &ttl);
    std::optional<std::string> unwrap(const Token &token);
    std::optional<std::string> lookup(const Token &token);
    std::optional<std::string> rewrap(const Token &token);
    std::optional<std::string> auditHash(const Path &path, const Parameters &parameters);
    std::optional<std::string> capabilities(const Parameters &parameters);
    std::optional<std::string> capabilitiesAccessor(const Parameters &parameters);
    std::optional<std::string> capabilitiesSelf(const Parameters &parameters);
    std::optional<std::string> state();
    std::optional<std::string> hostInfo();
    std::optional<std::string> keyStatus();
    std::optional<std::string> metrics();
    std::optional<std::string> reloadPlugin(const Parameters &parameters);
    std::optional<std::string> remount(const Parameters &parameters);
    std::optional<std::string> rotate();
    std::optional<std::string> seal();
    std::optional<std::string> sealStatus();
    std::optional<std::string> stepDown();

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

    class Audit {
    public:
      explicit Audit(const Client &client) : client_(client) {}

      std::optional<std::string> read();
      std::optional<std::string> enable(const Path &path, const Parameters &parameters);
      std::optional<std::string> disable(const Path &path);

    private:
      Url getUrl(const Path &path);

      const Client &client_;
    };

    class Auditing {
    public:
      explicit Auditing(const Client &client) : client_(client) {}

      std::optional<std::string> list();
      std::optional<std::string> read(const Path &path);
      std::optional<std::string> create(const Path &path, const Parameters &parameters);
      std::optional<std::string> update(const Path &path, const Parameters &parameters);
      std::optional<std::string> del(const Path &path);

    private:
      Url getUrl(const Path &path);

      const Client &client_;
    };

    class ControlGroup {
    public:
      explicit ControlGroup(const Client &client) : client_(client) {}

      std::optional<std::string> read();
      std::optional<std::string> configure(const Parameters &parameters);
      std::optional<std::string> del();
      std::optional<std::string> authorize(const Parameters &parameters);
      std::optional<std::string> request(const Parameters &parameters);

    private:
      Url getUrl(const Path &path);

      const Client &client_;
    };

    class Cors {
    public:
      explicit Cors(const Client &client) : client_(client) {}

      std::optional<std::string> read(const Path &path);
      std::optional<std::string> configure(const Path &path, const Parameters &parameters);
      std::optional<std::string> del(const Path &path);

    private:
      Url getUrl();

      const Client &client_;
    };

    class UI {
    public:
      explicit UI(const Client &client) : client_(client) {}

      std::optional<std::string> list();
      std::optional<std::string> read(const Path &path);
      std::optional<std::string> configure(const Path &path, const Parameters &parameters);
      std::optional<std::string> del(const Path &path);

    private:
      Url getUrl(const Path &path);

      const Client &client_;
    };

    class Init {
    public:
      explicit Init(const Client &client) : client_(client) {}

      std::optional<std::string> read();
      std::optional<std::string> start(const Parameters &parameters);

    private:
      Url getUrl();

      const Client &client_;
    };

    class Counters {
    public:
      explicit Counters(const Client &client) : client_(client) {}

      std::optional<std::string> requests();
      std::optional<std::string> entities();
      std::optional<std::string> tokens();

    private:
      Url getUrl(const Path &path);

      const Client &client_;
    };

    class Leases {
    public:
      explicit Leases(const Client &client) : client_(client) {}

      std::optional<std::string> read(const Parameters &parameters);
      std::optional<std::string> list(const Path &path);
      std::optional<std::string> renew(const Parameters &parameters);
      std::optional<std::string> revoke(const Parameters &parameters);
      std::optional<std::string> revokeForce(const Path &path, const Parameters &parameters);
      std::optional<std::string> revokePrefix(const Path &path, const Parameters &parameters);
      std::optional<std::string> tidy(const Parameters &parameters);

    private:
      Url getUrl(const Path &path);

      const Client &client_;
    };

    class License {
    public:
      explicit License(const Client &client) : client_(client) {}

      std::optional<std::string> read();
      std::optional<std::string> install(const Parameters &parameters);

    private:
      Url getUrl();

      const Client &client_;
    };

    class Mounts {
    public:
      explicit Mounts(const Client &client) : client_(client) {}

      std::optional<std::string> readUi();
      std::optional<std::string> read();
      std::optional<std::string> enable(const Path &path, const Parameters &parameters);

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

    std::optional<std::string> readCACertificate();
    std::optional<std::string> readCACertificateChain();
    std::optional<std::string> generateRoot(const RootCertificateType &rootCertificateType, const Parameters &parameters);
    std::optional<std::string> deleteRoot();
    std::optional<std::string> setUrls(const Parameters &parameters);
    std::optional<std::string> createRole(const Path &path, const Parameters &parameters);
    std::optional<std::string> updateRole(const Path &path, const Parameters &parameters);
    std::optional<std::string> readRole(const Path &path);
    std::optional<std::string> listRoles();
    std::optional<std::string> deleteRole(const Path &path);
    std::optional<std::string> issue(const Path &path, const Parameters &parameters);
    std::optional<std::string> listCertificates();
    std::optional<std::string> readCertificate(const Path &path);
    std::optional<std::string> configureCA(const Parameters &parameters);
    std::optional<std::string> readCrlConfiguration();
    std::optional<std::string> setCrlConfiguration(const Parameters &parameters);
    std::optional<std::string> readURLs();
    std::optional<std::string> setURLs(const Parameters &parameters);
    std::optional<std::string> readCRL();
    std::optional<std::string> rotateCrl();
    std::optional<std::string> generateIntermediate(const KeyType &keyType, const Parameters &parameters);
    std::optional<std::string> signIntermediate(const Parameters &parameters);
    std::optional<std::string> setSignedIntermediate(const Parameters &parameters);
    std::optional<std::string> signSelfIssued(const Parameters &parameters);
    std::optional<std::string> sign(const Path &path, const Parameters &parameters);
    std::optional<std::string> signVerbatim(const Path &path, const Parameters &parameters);
    std::optional<std::string> generateCertificate(const Path &path, const Parameters &parameters);
    std::optional<std::string> tidy(const Parameters &parameters);
    std::optional<std::string> revokeCertificate(const Parameters &parameters);

  private:
    Url getUrl(const Path &path);

    const Client &client_;
  };

  class RabbitMq {
  public:
    explicit RabbitMq(const Client &client) : client_(client) {}

    std::optional<std::string> configureConnection(const Parameters &parameters);
    std::optional<std::string> configureLease(const Parameters &parameters);
    std::optional<std::string> createRole(const Path &path, const Parameters &parameters);
    std::optional<std::string> readRole(const Path &path);
    std::optional<std::string> deleteRole(const Path &path);
    std::optional<std::string> generateCredentials(const Path &path);

  private:
    Url getUrl(const Path &path);

    const Client &client_;
  };

  class Database {
  public:
      explicit Database(const Client &client) : client_(client) {}

      std::optional<std::string> configureConnection(const Path &path, const Parameters &parameters);
      std::optional<std::string> readConnection(const Path &path);
      std::optional<std::string> listConnections();
      std::optional<std::string> deleteConnection(const Path &path);
      std::optional<std::string> resetConnection(const Path &path);
      std::optional<std::string> rotateRootCredentials(const Path &path);
      std::optional<std::string> createRole(const Path &path, const Parameters &parameters);
      std::optional<std::string> readRole(const Path &path);
      std::optional<std::string> listRoles();
      std::optional<std::string> deleteRole(const Path &path);
      std::optional<std::string> generateCredentials(const Path &path);
      std::optional<std::string> createStaticRole(const Path &path, const Parameters &parameters);
      std::optional<std::string> readStaticRole(const Path &path);
      std::optional<std::string> listStaticRoles();
      std::optional<std::string> deleteStaticRole(const Path &path);
      std::optional<std::string> getStaticCredentials(const Path &path);
      std::optional<std::string> rotateStaticCredentials(const Path &path);

  private:
      Url getUrl(const Path &path);

      const Client &client_;
  };

  class SSH {
  public:
    explicit SSH(const Client &client) : client_(client) {}

    std::optional<std::string> createKey(const Path &path, const Parameters &parameters);
    std::optional<std::string> updateKey(const Path &path, const Parameters &parameters);
    std::optional<std::string> deleteKey(const Path &path);
    std::optional<std::string> createRole(const Path &path, const Parameters &parameters);
    std::optional<std::string> readRole(const Path &path);
    std::optional<std::string> listRoles();
    std::optional<std::string> deleteRole(const Path &path);
    std::optional<std::string> listZeroAddressRoles();
    std::optional<std::string> configureZeroAddressRoles(const Parameters &parameters);
    std::optional<std::string> deleteZeroAddressRole();
    std::optional<std::string> generateCredentials(const Path &path, const Parameters &parameters);
    std::optional<std::string> listRolesByIp(const Parameters &parameters);
    std::optional<std::string> verifyOtp(const Parameters &parameters);
    std::optional<std::string> configureCA(const Parameters &parameters);
    std::optional<std::string> deleteCA();
    std::optional<std::string> readPublicKey();
    std::optional<std::string> signKey(const Path &path, const Parameters &parameters);

  private:
    Url getUrl(const Path &path);

    const Client &client_;
  };

  class ActiveDirectory {
  public:
    explicit ActiveDirectory(const Client &client) : client_(client) {}

    std::optional<std::string> createConfig(const Parameters &parameters);
    std::optional<std::string> readConfig();
    std::optional<std::string> deleteConfig();
    std::optional<std::string> readRoles();
    std::optional<std::string> readRole(const Path &path);
    std::optional<std::string> createRole(const Path &path, const Parameters &parameters);
    std::optional<std::string> deleteRole(const Path &path);
    std::optional<std::string> readCredentials(const Path &path);
    std::optional<std::string> readServiceAccounts();
    std::optional<std::string> readServiceAccount(const Path &path);
    std::optional<std::string> createServiceAccount(const Path &path, const Parameters &parameters);
    std::optional<std::string> deleteServiceAccount(const Path &path);
    std::optional<std::string> checkout(const Path &path, const Parameters &parameters);
    std::optional<std::string> checkin(const Path &path, const Parameters &parameters);
    std::optional<std::string> manageCheckin(const Path &path, const Parameters &parameters);
    std::optional<std::string> status(const Path &path);
    std::optional<std::string> rotateRootCredentials();

  private:
    Url getUrl(const Path &path);

    const Client &client_;
  };

  class AliCloud {
    class Auth {
    public:
      explicit Auth(const Client &client) : client_(client) {}

      std::optional<std::string> createRole(const Path &path, const Parameters &parameters);
      std::optional<std::string> readRole(const Path &path);
      std::optional<std::string> listRoles();
      std::optional<std::string> deleteRole(const Path &path);
      std::optional<std::string> login(const Parameters &parameters);

    private:
      Url getUrl(const Path &path);

      const Client &client_;

    };

    class Secrets {
    public:
      explicit Secrets(const Client &client) : client_(client) {}

      std::optional<std::string> readConfig();
      std::optional<std::string> createConfig(const Parameters &parameters);
      std::optional<std::string> listRoles();
      std::optional<std::string> readRole(const Path &path);
      std::optional<std::string> createRole(const Path &path, const Parameters &parameters);
      std::optional<std::string> deleteRole(const Path &path);
      std::optional<std::string> generateCredentials(const Path &path);

    private:
      Url getUrl(const Path &path);

      const Client &client_;
    };
  };
}

#pragma GCC diagnostic pop
