#include "../../shared/shared.h"
#include "jwt.h"
#include <filesystem>
#include <fstream>
#include <iostream>

std::string read(std::filesystem::path path) {
  std::ifstream file(path.c_str());
  return std::string(std::istreambuf_iterator<char>(file), {});
}

Vault::Jwt makeJwt(const std::filesystem::path &publicKeyPath,
                   const std::filesystem::path &privateKeyPath) {
  std::string publicKeyString = read(publicKeyPath);
  std::string privateKeyString = read(privateKeyPath);
  std::string token =
      jwt::create()
          .set_type("JWT")
          .set_audience("example")
          .set_payload_claim("example", jwt::claim(std::string{"example"}))
          .set_issued_at(std::chrono::system_clock::now())
          .set_expires_at(std::chrono::system_clock::now() +
                          std::chrono::seconds{60})
          .sign(
              jwt::algorithm::rs256(publicKeyString, privateKeyString, "", ""));

  return Vault::Jwt{token};
}

Vault::Client setup(const Vault::Client &rootClient,
                    const std::filesystem::path &publicKeyPath,
                    const std::filesystem::path &privateKeyPath,
                    const Vault::Path &path) {
  Vault::Sys::Auth authAdmin{rootClient};
  Vault::JwtOidc jwtAdmin{rootClient, path};

  enableJwtAuthentication(authAdmin, path);
  createRole(jwtAdmin);
  configureJwtAuthentication(jwtAdmin, read(publicKeyPath));

  Vault::RoleId role{"example"};
  Vault::Jwt jwt = makeJwt(publicKeyPath, privateKeyPath);

  return getJwtClient(role, jwt, path);
}

void cleanup(const Vault::Client &rootClient, const Vault::Path &path) {
  Vault::Sys::Auth authAdmin{rootClient};
  Vault::JwtOidc jwtAdmin{rootClient, path};

  deleteRole(jwtAdmin);
  disableJwtAuthentication(authAdmin, path);
}

int main(void) {
  char *rootTokenEnv = std::getenv("VAULT_ROOT_TOKEN");
  if (!rootTokenEnv) {
    std::cout << "The VAULT_TOKEN environment variable must be set"
              << std::endl;
    exit(-1);
  }
  Vault::Token rootToken{rootTokenEnv};
  Vault::Client rootClient = getRootClient(rootToken);
  Vault::Path path{"jwt"};
  Vault::Client jwtClient =
      setup(rootClient, 
            std::filesystem::path{"public.pem"},
            std::filesystem::path{"private.pem"},
            path
      );

  if (jwtClient.is_authenticated()) {
    std::cout << "Authenticated: " << jwtClient.getToken() << std::endl;
  } else {
    std::cout << "Unable to authenticate" << std::endl;
  }

  cleanup(rootClient, path);
}
