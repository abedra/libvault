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

  pki.generateRoot(Vault::RootCertificateTypes::INTERNAL, rootCertificateParameters);
  pki.setUrls(urlParameters);
  pki.createRole(Vault::Path{"example-dot-com"}, roleParameters);

  SECTION("Issue")
  {
    Vault::Path path("example-dot-com");
    Vault::Parameters parameters({{"common_name", "www.my-website.com"}});

    auto response = pki.issue(path, parameters);

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];

      CHECK(data["private_key_type"] == "rsa");
      CHECK(!data["certificate"].empty());
      CHECK(!data["private_key"].empty());
      CHECK(!data["serial_number"].empty());
    } else {
      CHECK(false);
    }
  }
}
