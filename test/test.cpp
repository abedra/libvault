#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <experimental/optional>

#include "VaultClient.h"

class FailedAuth : public AuthenticationStrategy {
public:
  std::experimental::optional<std::string> authenticate(const VaultClient& client) override {
    return std::experimental::nullopt;
  }
private:
};

class SuccessfulAuth : public AuthenticationStrategy {
public:
  std::experimental::optional<std::string> authenticate(const VaultClient& client) override {
    return std::experimental::optional<std::string>("success");
  }
private:
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
  REQUIRE(vaultClient.getUrl("/base", "/path") == "https://localhost:8200/base/path");
}

TEST_CASE("VaultClient#getUrl tls false")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient(VaultConfigBuilder().withTlsEnabled(false).build(), strategy);
  REQUIRE(vaultClient.getUrl("/base", "/path") == "http://localhost:8200/base/path");
}

TEST_CASE("VaultClient#getToken")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient(config, strategy);
  REQUIRE(vaultClient.getToken() == "success");
}

TEST_CASE("VaultClient#getNamespace")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient(VaultConfigBuilder().withNamespace("ns").build(), strategy);
  REQUIRE(vaultClient.getNamespace() == "ns");
}

TEST_CASE("HttpClient#is_success when response is empty")
{
  auto response = std::experimental::nullopt;
  REQUIRE(!HttpClient::is_success(response));
}

TEST_CASE("HttpClient#is_success when status code not 200")
{
  auto response = std::experimental::optional<HttpResponse>({403, "Permission Denied"});
  REQUIRE(!HttpClient::is_success(response));
}

TEST_CASE("HttpClient#is_success when status 200")
{
  auto response = std::experimental::optional<HttpResponse>({200, "OK"});
  REQUIRE(HttpClient::is_success(response));
}

TEST_CASE("VaultConfig#make default")
{
  auto config = VaultConfigBuilder().build();

  REQUIRE(config.getHost() == "localhost");
  REQUIRE(config.getPort() == "8200");
  REQUIRE(config.getTls() == true);
  REQUIRE(config.getDebug() == false);
  REQUIRE(config.getConnectTimeout() == 10);
}

TEST_CASE("VaultConfig#make options set")
{
  auto config = VaultConfigBuilder()
    .withHost("example.com")
    .withPort("8100")
    .withTlsVerification(false)
    .withTlsEnabled(false)
    .withDebug(true)
    .withConnectTimeout(5)
    .build();

  REQUIRE(config.getHost() == "example.com");
  REQUIRE(config.getPort() == "8100");
  REQUIRE(config.getTls() == false);
  REQUIRE(config.getDebug() == true);
  REQUIRE(config.getVerify() == false);
  REQUIRE(config.getConnectTimeout() == 5);
}
