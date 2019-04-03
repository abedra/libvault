#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <experimental/optional>

#include "VaultClient.h"

class FailedAuth : public AuthenticationStrategy {
public:
  std::experimental::optional<std::string> authenticate(VaultClient* client) {
    return std::experimental::nullopt;
  }
private:
};

class SuccessfulAuth : public AuthenticationStrategy {
public:
  std::experimental::optional<std::string> authenticate(VaultClient* client) {
    return std::experimental::optional<std::string>("success");
  }
private:
};

TEST_CASE("VaultClient#isAuthenticated failure")
{
  auto strategy = FailedAuth();
  auto vaultClient = VaultClient("localhost", "8200", strategy);
  REQUIRE(vaultClient.isAuthenticated() == false);
}

TEST_CASE("VaultClient#isAuthenticated success")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient("localhost", "8200", strategy);
  REQUIRE(vaultClient.isAuthenticated() == true);
}

TEST_CASE("VaultClient#getUrl")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient("localhost", "8200", strategy);
  REQUIRE(vaultClient.getUrl("/base", "/path") == "http://localhost:8200/base/path");
}

TEST_CASE("VaultClient#getToken")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient("localhost", "8200", strategy);
  REQUIRE(vaultClient.getToken() == "success");
}

TEST_CASE("VaultClient#getNamespace")
{
  auto strategy = SuccessfulAuth();
  auto vaultClient = VaultClient("localhost", "8200", strategy);
  vaultClient.setNamespace("ns");
  REQUIRE(vaultClient.getNamespace() == "ns");
}

TEST_CASE("HttpClient#is_success when response is empty")
{
  auto response = std::experimental::nullopt;
  REQUIRE(HttpClient::is_success(response) == false);
}

TEST_CASE("HttpClient#is_success when status code not 200")
{
  auto response = std::experimental::optional<HttpResponse>({403, "Permission Denied"});
  REQUIRE(HttpClient::is_success(response) == false);
}

TEST_CASE("HttpClient#is_success when status 200")
{
  auto response = std::experimental::optional<HttpResponse>({200, "OK"});
  REQUIRE(HttpClient::is_success(response) == true);
}
