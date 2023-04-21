#include <catch2/catch.hpp>

#include "../TestHelpers.h"
#include "VaultClient.h"

TEST_CASE("Pki") {
  Vault::Client vaultClient = TestHelpers::AppRole::login();
  Vault::Pki pki(vaultClient);
  Vault::Parameters rootCertificateParameters(
      {{"common_name", "my-website.com"}, {"ttl", "8760h"}});
  Vault::Parameters urlParameters(
      {{"issuing_certificates", "http://127.0.0.1:8200/v1/pki/ca"},
       {"crl_distribution_points", "http://127.0.0.1:8200/v1/pki/crl"}});
  Vault::Parameters roleParameters({{"allowed_domains", "my-website.com"},
                                    {"allow_subdomains", "true"},
                                    {"max_ttl", "72h"}});

  auto generateRootResponse = pki.generateRoot(
      Vault::RootCertificateTypes::INTERNAL, rootCertificateParameters);
  if (!generateRootResponse) {
    REQUIRE(false);
  }

  pki.setUrls(urlParameters);
  pki.createRole(Vault::Path{"example-dot-com"}, roleParameters);

  SECTION("Issue/Revoke Certificate") {
    Vault::Path path("example-dot-com");
    Vault::Parameters parameters({{"common_name", "www.my-website.com"}});

    auto issueResponse = pki.issue(path, parameters);

    if (issueResponse) {
      auto data = nlohmann::json::parse(issueResponse.value())["data"];

      REQUIRE(data["private_key_type"] == "rsa");
      REQUIRE(!data["certificate"].empty());
      REQUIRE(!data["private_key"].empty());
      REQUIRE(!data["serial_number"].empty());

      Vault::Parameters revokeParameters(
          {{"serial_number", std::string(data["serial_number"])}});
      auto revokeResponse = pki.revokeCertificate(revokeParameters);

      if (revokeResponse) {
        auto revocation_time = nlohmann::json::parse(
            revokeResponse.value())["data"]["revocation_time"];
        REQUIRE(revocation_time > 0);
      } else {
        REQUIRE(false);
      }
    } else {
      REQUIRE(false);
    }
  }

  SECTION("List Certificates") {
    auto response = pki.listCertificates();

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      REQUIRE(!data["keys"].empty());
    } else {
      REQUIRE(false);
    }
  }

  SECTION("Read Certificate") {
    auto response = pki.readCertificate(Vault::Path{"crl"});

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      REQUIRE(!data.empty());
    } else {
      REQUIRE(false);
    }
  }

  SECTION("Read CRL Configuration") {
    Vault::Parameters parameters({{"expiry", "72h"}});
    pki.setCrlConfiguration(parameters);

    auto response = pki.readCrlConfiguration();

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      REQUIRE(data["disable"] == false);
      REQUIRE(data["expiry"] == "72h");
    } else {
      REQUIRE(false);
    }
  }

  SECTION("Rotate CRL") {
    auto response = pki.rotateCrl();

    if (response) {
      auto success = nlohmann::json::parse(response.value())["data"]["success"];
      REQUIRE(success == true);
    } else {
      REQUIRE(false);
    }
  }
}
