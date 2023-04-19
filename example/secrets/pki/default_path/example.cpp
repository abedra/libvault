#include "../../../../lib/json.hpp"
#include "../../../shared/shared.h"
#include "VaultClient.h"
#include <iostream>

void setup(const Vault::Pki &pkiAdmin) {
  Vault::Parameters rootCertificateParameters(
      {{"common_name", "my-website.com"}, {"ttl", "8760h"}});
  Vault::Parameters urlParameters(
      {{"issuing_certificates", "http://127.0.0.1:8200/v1/pki/ca"},
       {"crl_distribution_points", "http://127.0.0.1:8200/v1/pki/crl"}});
  Vault::Parameters roleParameters({{"allowed_domains", "my-website.com"},
                                    {"allow_subdomains", "true"},
                                    {"max_ttl", "72h"}});

  auto rootCertificateResponse = pkiAdmin.generateRoot(
      Vault::RootCertificateTypes::INTERNAL, rootCertificateParameters);
  if (!rootCertificateResponse) {
    std::cout << "Could not generate root certificate" << std::endl;
    exit(-1);
  }

  pkiAdmin.setUrls(urlParameters);
  pkiAdmin.createRole(Vault::Path{"example-dot-com"}, roleParameters);
}

int main(void) {
  char *rootTokenEnv = std::getenv("VAULT_ROOT_TOKEN");
  if (!rootTokenEnv) {
    std::cout << "The VAULT_ROOT_TOKEN environment variable must be set"
              << std::endl;
    exit(-1);
  }
  Vault::Token rootToken{rootTokenEnv};
  Vault::Client rootClient = getRootClient(rootToken);
  Vault::Sys::Mounts mountAdmin{rootClient};
  Vault::SecretMount mount{"pki"};

  if (rootClient.is_authenticated()) {
    enablePki(mountAdmin, mount);
    Vault::Pki pkiAdmin{rootClient};
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

  disablePki(mountAdmin, mount);
}
