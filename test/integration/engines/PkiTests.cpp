#include <catch2/catch.hpp>

#include "VaultClient.h"
#include "../TestHelpers.h"

TEST_CASE("Pki")
{
  Vault::Client vaultClient = TestHelpers::AppRole::login();
  Vault::Pki pki(vaultClient);
  Vault::Parameters rootCertificateParameters({
    {"common_name", "my-website.com"},
    {"ttl", "8760h"}
  });
  Vault::Parameters urlParameters({
    {"issuing_certificates", "http://127.0.0.1:8200/v1/pki/ca"},
    {"crl_distribution_points", "http://127.0.0.1:8200/v1/pki/crl"}
  });
  Vault::Parameters roleParameters({
    {"allowed_domains", "my-website.com"},
    {"allow_subdomains", "true"},
    {"max_ttl", "72h"}
  });

  auto generateRootResponse = pki.generateRoot(Vault::RootCertificateTypes::INTERNAL, rootCertificateParameters);
  if (!generateRootResponse) {
    REQUIRE(false);
  }

  pki.setUrls(urlParameters);
  pki.createRole(Vault::Path{"example-dot-com"}, roleParameters);

  SECTION("Issue/Revoke Certificate")
  {
    Vault::Path path("example-dot-com");
    Vault::Parameters parameters({{"common_name", "www.my-website.com"}});

    auto issueResponse = pki.issue(path, parameters);

    if (issueResponse) {
      auto data = nlohmann::json::parse(issueResponse.value())["data"];

      CHECK(data["private_key_type"] == "rsa");
      CHECK(!data["certificate"].empty());
      CHECK(!data["private_key"].empty());
      CHECK(!data["serial_number"].empty());

      Vault::Parameters revokeParameters({{"serial_number", data["serial_number"]}});
      auto revokeResponse = pki.revokeCertificate(revokeParameters);

      if (revokeResponse) {
        auto revocation_time = nlohmann::json::parse(revokeResponse.value())["data"]["revocation_time"];
        CHECK(revocation_time > 0);
      } else {
        CHECK(false);
      }
    } else {
      CHECK(false);
    }
  }

  SECTION("List Certificates")
  {
    auto response = pki.listCertificates();

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      CHECK(!data["keys"].empty());
    } else {
      CHECK(false);
    }
  }

  SECTION("Read Certificate")
  {
    auto response = pki.readCertificate(Vault::Path{"crl"});

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      CHECK(!data.empty());
    } else {
      CHECK(false);
    }
  }

  SECTION("Read CRL Configuration")
  {
    Vault::Parameters parameters({{"expiry", "72h"}});
    pki.setCrlConfiguration(parameters);

    auto response = pki.readCrlConfiguration();

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      CHECK(data["disable"] == false);
      CHECK(data["expiry"] == "72h");
    } else {
      CHECK(false);
    }
  }

  SECTION("Rotate CRL")
  {
    auto response = pki.rotateCrl();

    if (response) {
      auto success = nlohmann::json::parse(response.value())["data"]["success"];
      CHECK(success == true);
    } else {
      CHECK(false);
    }
  }
}
