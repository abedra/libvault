#include <iostream>
#include "../../../../lib/json.hpp"
#include "VaultClient.h"

void setup(const Vault::Pki &pkiAdmin) {
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

  auto rootCertificateResponse = pkiAdmin.generateRoot(Vault::RootCertificateTypes::INTERNAL, rootCertificateParameters);
  if (!rootCertificateResponse) {
    std::cout << "Could not generate root certificate" << std::endl;
    exit(-1);
  }

  pkiAdmin.setUrls(urlParameters);
  pkiAdmin.createRole(Vault::Path{"example-dot-com"}, roleParameters);
}

int main(void) {
  char *roleId = std::getenv("APPROLE_ROLE_ID");
  char *secretId = std::getenv("APPROLE_SECRET_ID");

  if (!roleId && !secretId) {
    std::cout << "APPROLE_ROLE_ID and APPROLE_SECRET_ID environment variables must be set" << std::endl;
    exit(-1);
  }

  Vault::AppRoleStrategy appRoleStrategy{Vault::RoleId{roleId}, Vault::SecretId{secretId}};
  Vault::Config config = Vault::ConfigBuilder().withDebug(false).withTlsEnabled(false).build();
  Vault::Client vaultClient = Vault::Client{config, appRoleStrategy};

  if (vaultClient.is_authenticated()) {
    Vault::Pki pkiAdmin{vaultClient, Vault::SecretMount{"certificates"}};
    setup(pkiAdmin);

    Vault::Path path("example-dot-com");
    Vault::Parameters parameters({{"common_name", "www.my-website.com"}});

    auto response = pkiAdmin.issue(path, parameters);

    if (response) {
      auto data = nlohmann::json::parse(response.value())["data"];
      std::cout << data["certificate"] << std::endl << std::endl;
      std::cout << data["private_key"] << std::endl;
    } else {
      std::cout << "Could not issue certificate" << std::endl;
    }
  } else {
    std::cout << "Unable to authenticate to Vault" << std::endl;
  }
}
