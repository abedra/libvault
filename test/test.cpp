#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <optional>

#include "VaultClient.h"

class FailedAuth : public AuthenticationStrategy {
public:
  std::optional<AuthenticationResponse> authenticate(const VaultClient& client) override {
    return std::nullopt;
  }
private:
};

class SuccessfulAuth : public AuthenticationStrategy {
public:
  std::optional<AuthenticationResponse> authenticate(const VaultClient& client) override {
    return std::optional<AuthenticationResponse>({HttpResponseBodyString{""}, Token{"success"}});
  }
private:
};

class MockHttpClient : public HttpClient {
public:
  explicit MockHttpClient(VaultConfig& config) : HttpClient(config) {}

  void SetResponse(std::optional<HttpResponse> response) {
    response_ = response;
  }

  [[nodiscard]] std::optional<HttpResponse> get(const Url& url, const Token& token, const Namespace& ns) const override {
    return response_;
  }
  [[nodiscard]] std::optional<HttpResponse> post(const Url& url, const Token& token, const Namespace& ns, std::string value) const override {
    return response_;
  }
  [[nodiscard]] std::optional<HttpResponse> del(const Url& url, const Token& token, const Namespace& ns) const override {
    return response_;
  }
  [[nodiscard]] std::optional<HttpResponse> list(const Url& url, const Token& token, const Namespace& ns) const override {
    return response_;
  }
private:
  std::optional<HttpResponse> response_;
};

class MockVaultClient : public VaultClient {
public:
  MockVaultClient(VaultConfig& config, AuthenticationStrategy& authStrategy, const MockHttpClient& mockHttpClient)
    : VaultClient(config, authStrategy), mockHttpClient_(mockHttpClient) {}

  [[nodiscard]] const HttpClient& getHttpClient() const override {
    return mockHttpClient_;
  }
private:
  const MockHttpClient& mockHttpClient_;
};

auto config = VaultConfigBuilder().build();

TEST_CASE("VaultClient#is_authenticated failure")
{
  auto strategy = FailedAuth();
  auto vaultClient = VaultClient(config, strategy);
  REQUIRE(!vaultClient.is_authenticated());
}

TEST_CASE("VaultClient#is_authenticated success")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient(config, strategy);
  REQUIRE(vaultClient.is_authenticated());
}

TEST_CASE("VaultClient#getUrl")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient(config, strategy);
  REQUIRE(vaultClient.getUrl("/base", Path{"/path"}).value() == "https://localhost:8200/base/path");
}

TEST_CASE("VaultClient#getUrl tls false")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient(VaultConfigBuilder().withTlsEnabled(false).build(), strategy);
  REQUIRE(vaultClient.getUrl("/base", Path{"/path"}).value() == "http://localhost:8200/base/path");
}

TEST_CASE("VaultClient#getToken")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient(config, strategy);
  REQUIRE(vaultClient.getToken().value() == "success");
}

TEST_CASE("VaultClient#getNamespace")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient(VaultConfigBuilder().withNamespace(Namespace{"ns"}).build(), strategy);
  REQUIRE(vaultClient.getNamespace().value() == "ns");
}

TEST_CASE("HttpClient#is_success when response is empty")
{
  auto response = std::nullopt;
  REQUIRE(!HttpClient::is_success(response));
}

TEST_CASE("HttpClient#is_success when status code not 200")
{
  auto response = std::optional<HttpResponse>({403, HttpResponseBodyString{"Permission Denied"}});
  REQUIRE(!HttpClient::is_success(response));
}

TEST_CASE("HttpClient#is_success when status 200")
{
  auto response = std::optional<HttpResponse>({200, HttpResponseBodyString{"OK"}});
  REQUIRE(HttpClient::is_success(response));
}

TEST_CASE("VaultConfig#make default")
{
  config= VaultConfigBuilder().build();

  REQUIRE(config.getHost().value() == "localhost");
  REQUIRE(config.getPort().value() == "8200");
  REQUIRE(config.getTls() == true);
  REQUIRE(config.getDebug() == false);
  REQUIRE(config.getConnectTimeout().value == 10);
}

TEST_CASE("VaultConfig#make options set")
{
  config= VaultConfigBuilder()
    .withHost(VaultHost{"example.com"})
    .withPort(VaultPort{"8100"})
    .withTlsVerification(false)
    .withTlsEnabled(false)
    .withDebug(true)
    .withConnectTimeout(VaultConnectTimeout{5})
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
  auto httpClient = MockHttpClient(VaultConfigBuilder().build());

  httpClient.SetResponse(std::nullopt);
  REQUIRE(httpClient.get(Url("/test"), Token("foo"), Namespace("bar")) == std::nullopt);
  REQUIRE(httpClient.post(Url("/test"), Token("foo"), Namespace("bar"), "baz") == std::nullopt);
  REQUIRE(httpClient.del(Url("/test"), Token("foo"), Namespace("bar")) == std::nullopt);
  REQUIRE(httpClient.list(Url("/test"), Token("foo"), Namespace("bar")) == std::nullopt);

  auto resp = HttpResponse{200, HttpResponseBodyString("success")};
  httpClient.SetResponse(resp);
  auto getResp = httpClient.get(Url("/test"), Token("foo"), Namespace("bar"));
  REQUIRE(getResp.has_value());
  REQUIRE(getResp->statusCode.value == resp.statusCode.value);
  REQUIRE(getResp->body.value() == resp.body.value());
  auto postResp = httpClient.post(Url("/test"), Token("foo"), Namespace("bar"), "baz");
  REQUIRE(postResp.has_value());
  REQUIRE(postResp->statusCode.value == resp.statusCode.value);
  REQUIRE(postResp->body.value() == resp.body.value());
  auto delResp = httpClient.del(Url("/test"), Token("foo"), Namespace("bar"));
  REQUIRE(delResp.has_value());
  REQUIRE(delResp->statusCode.value == resp.statusCode.value);
  REQUIRE(delResp->body.value() == resp.body.value());
  auto listResp = httpClient.list(Url("/test"), Token("foo"), Namespace("bar"));
  REQUIRE(listResp.has_value());
  REQUIRE(listResp->statusCode.value == resp.statusCode.value);
  REQUIRE(listResp->body.value() == resp.body.value());
}

TEST_CASE("MockVaultClient#mock getHttpClient")
{
  auto config = VaultConfigBuilder().build();
  auto strategy = SuccessfulAuth();
  auto httpClient = MockHttpClient(config);
  auto vaultClient = MockVaultClient(config, strategy, httpClient);

  REQUIRE(&(vaultClient.getHttpClient()) == &httpClient);
}
