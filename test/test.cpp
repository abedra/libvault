#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <optional>

#include "VaultClient.h"

class FailedAuth : public Vault::AuthenticationStrategy {
public:
  std::optional<Vault::AuthenticationResponse> authenticate(const Vault::Client& client) override {
    return std::nullopt;
  }
private:
};

class SuccessfulAuth : public Vault::AuthenticationStrategy {
public:
  std::optional<Vault::AuthenticationResponse> authenticate(const Vault::Client& client) override {
    return std::optional<Vault::AuthenticationResponse>({Vault::HttpResponseBodyString{""}, Vault::Token{"success"}});
  }
private:
};

class MockHttpClient : public Vault::HttpClient {
public:
  explicit MockHttpClient(Vault::Config& config) : HttpClient(config) {}

  void SetResponse(std::optional<Vault::HttpResponse> response) {
    response_ = response;
  }

  [[nodiscard]] std::optional<Vault::HttpResponse> get(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns) const override {
    return response_;
  }
  [[nodiscard]] std::optional<Vault::HttpResponse> post(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns, std::string value) const override {
    return response_;
  }
  [[nodiscard]] std::optional<Vault::HttpResponse> del(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns) const override {
    return response_;
  }
  [[nodiscard]] std::optional<Vault::HttpResponse> list(const Vault::Url& url, const Vault::Token& token, const Vault::Namespace& ns) const override {
    return response_;
  }
private:
  std::optional<Vault::HttpResponse> response_;
};

class MockVaultClient : public Vault::Client {
public:
  MockVaultClient(Vault::Config& config, Vault::AuthenticationStrategy& authStrategy, const MockHttpClient& mockHttpClient)
    : Vault::Client(config, authStrategy)
    , mockHttpClient_(mockHttpClient)
    {}

  [[nodiscard]] const Vault::HttpClient& getHttpClient() const override {
    return mockHttpClient_;
  }
private:
  const MockHttpClient& mockHttpClient_;
};

auto config = Vault::ConfigBuilder().build();

TEST_CASE("VaultClient#is_authenticated failure")
{
  auto strategy = FailedAuth();
  auto vaultClient = Vault::Client(config, strategy);
  REQUIRE(!vaultClient.is_authenticated());
}

TEST_CASE("VaultClient#is_authenticated success")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = Vault::Client(config, strategy);
  REQUIRE(vaultClient.is_authenticated());
}

TEST_CASE("VaultClient#getUrl")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = Vault::Client(config, strategy);
  REQUIRE(vaultClient.getUrl("/base", Vault::Path{"/path"}).value() == "https://localhost:8200/base/path");
}

TEST_CASE("VaultClient#getUrl tls false")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = Vault::Client(Vault::ConfigBuilder().withTlsEnabled(false).build(), strategy);
  REQUIRE(vaultClient.getUrl("/base", Vault::Path{"/path"}).value() == "http://localhost:8200/base/path");
}

TEST_CASE("VaultClient#getToken")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = Vault::Client(config, strategy);
  REQUIRE(vaultClient.getToken().value() == "success");
}

TEST_CASE("VaultClient#getNamespace")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = Vault::Client(Vault::ConfigBuilder().withNamespace(Vault::Namespace{"ns"}).build(), strategy);
  REQUIRE(vaultClient.getNamespace().value() == "ns");
}

TEST_CASE("HttpClient#is_success when response is empty")
{
  auto response = std::nullopt;
  REQUIRE(!Vault::HttpClient::is_success(response));
}

TEST_CASE("HttpClient#is_success when status code not 200")
{
  auto response = std::optional<Vault::HttpResponse>({403, Vault::HttpResponseBodyString{"Permission Denied"}});
  REQUIRE(!Vault::HttpClient::is_success(response));
}

TEST_CASE("HttpClient#is_success when status 200")
{
  auto response = std::optional<Vault::HttpResponse>({200, Vault::HttpResponseBodyString{"OK"}});
  REQUIRE(Vault::HttpClient::is_success(response));
}

TEST_CASE("VaultConfig#make default")
{
  config= Vault::ConfigBuilder().build();

  REQUIRE(config.getHost().value() == "localhost");
  REQUIRE(config.getPort().value() == "8200");
  REQUIRE(config.getTls() == true);
  REQUIRE(config.getDebug() == false);
  REQUIRE(config.getConnectTimeout().value == 10);
}

TEST_CASE("VaultConfig#make options set")
{
  config= Vault::ConfigBuilder()
    .withHost(Vault::Host{"example.com"})
    .withPort(Vault::Port{"8100"})
    .withTlsVerification(false)
    .withTlsEnabled(false)
    .withDebug(true)
    .withConnectTimeout(Vault::ConnectTimeout{5})
    .build();

  REQUIRE(config.getHost().value() == "example.com");
  REQUIRE(config.getPort().value() == "8100");
  REQUIRE(config.getTls() == false);
  REQUIRE(config.getDebug() == true);
  REQUIRE(config.getVerify() == false);
  REQUIRE(config.getConnectTimeout().value == 5);
}

TEST_CASE("MockHttpClient#return mocked response")
{
  auto httpClient = MockHttpClient(Vault::ConfigBuilder().build());

  httpClient.SetResponse(std::nullopt);
  REQUIRE(httpClient.get(Vault::Url("/test"), Vault::Token("foo"), Vault::Namespace("bar")) == std::nullopt);
  REQUIRE(httpClient.post(Vault::Url("/test"), Vault::Token("foo"), Vault::Namespace("bar"), "baz") == std::nullopt);
  REQUIRE(httpClient.del(Vault::Url("/test"), Vault::Token("foo"), Vault::Namespace("bar")) == std::nullopt);
  REQUIRE(httpClient.list(Vault::Url("/test"), Vault::Token("foo"), Vault::Namespace("bar")) == std::nullopt);

  auto resp = Vault::HttpResponse{200, Vault::HttpResponseBodyString("success")};
  httpClient.SetResponse(resp);
  auto getResp = httpClient.get(Vault::Url("/test"), Vault::Token("foo"), Vault::Namespace("bar"));
  REQUIRE(getResp.has_value());
  REQUIRE(getResp->statusCode.value == resp.statusCode.value);
  REQUIRE(getResp->body.value() == resp.body.value());
  auto postResp = httpClient.post(Vault::Url("/test"), Vault::Token("foo"), Vault::Namespace("bar"), "baz");
  REQUIRE(postResp.has_value());
  REQUIRE(postResp->statusCode.value == resp.statusCode.value);
  REQUIRE(postResp->body.value() == resp.body.value());
  auto delResp = httpClient.del(Vault::Url("/test"), Vault::Token("foo"), Vault::Namespace("bar"));
  REQUIRE(delResp.has_value());
  REQUIRE(delResp->statusCode.value == resp.statusCode.value);
  REQUIRE(delResp->body.value() == resp.body.value());
  auto listResp = httpClient.list(Vault::Url("/test"), Vault::Token("foo"), Vault::Namespace("bar"));
  REQUIRE(listResp.has_value());
  REQUIRE(listResp->statusCode.value == resp.statusCode.value);
  REQUIRE(listResp->body.value() == resp.body.value());
}

TEST_CASE("MockVaultClient#mock getHttpClient")
{
  auto config = Vault::ConfigBuilder().build();
  auto strategy = SuccessfulAuth();
  auto httpClient = MockHttpClient(config);
  auto vaultClient = MockVaultClient(config, strategy, httpClient);

  REQUIRE(&(vaultClient.getHttpClient()) == &httpClient);
}
