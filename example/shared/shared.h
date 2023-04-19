#pragma once
#include "../../lib/json.hpp"
#include "VaultClient.h"
#include <iostream>

inline Vault::Client getRootClient(const Vault::Token &rootToken) {
  Vault::TokenStrategy tokenStrategy{rootToken};
  Vault::Config config =
      Vault::ConfigBuilder().withDebug(false).withTlsEnabled(false).build();
  Vault::HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };
  Vault::ResponseErrorCallback responseCallback = [&](Vault::HttpResponse err) {
    std::cout << err.statusCode << " : " << err.url.value() << " : "
              << err.body.value() << std::endl;
  };
  return Vault::Client{config, tokenStrategy, httpErrorCallback,
                       responseCallback};
}

inline Vault::Client getAppRoleClient(const Vault::RoleId &roleId,
                                      const Vault::SecretId &secretId,
                                      const Vault::Path &mount) {
  Vault::AppRoleStrategy authStrategy{roleId, secretId, mount};
  Vault::Config config = Vault::ConfigBuilder().withTlsEnabled(false).build();

  return Vault::Client{config, authStrategy};
}

inline Vault::Client getJwtClient(const Vault::RoleId &role,
                                  const Vault::Jwt &jwt) {
  Vault::JwtStrategy authStrategy{role, jwt};
  Vault::Config config =
      Vault::ConfigBuilder().withDebug(false).withTlsEnabled(false).build();
  Vault::HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };
  Vault::ResponseErrorCallback responseCallback = [&](Vault::HttpResponse err) {
    std::cout << err.statusCode << " : " << err.url.value() << " : "
              << err.body.value() << std::endl;
  };

  return Vault::Client{config, authStrategy, httpErrorCallback,
                       responseCallback};
}

inline std::optional<std::string>
createPolicy(const Vault::Sys::Policy &policyAdmin) {
  Vault::Parameters parameters{
      {"policy", "path \"kv/*\" {capabilities = [\"read\", \"update\", "
                 "\"list\", \"delete\", \"create\"]}"}};
  return policyAdmin.create(Vault::Path{"example"}, parameters);
}

inline std::optional<std::string>
deletePolicy(const Vault::Sys::Policy &policyAdmin) {
  return policyAdmin.del(Vault::Path{"example"});
}

inline std::optional<std::string>
enableAppRole(const Vault::Sys::Auth &authAdmin, const Vault::Path &mount) {
  return authAdmin.enable(mount, Vault::Parameters{{"type", "approle"}});
}

inline std::optional<std::string>
disableAppRole(const Vault::Sys::Auth &authAdmin, const Vault::Path &mount) {
  return authAdmin.disable(mount);
}

inline std::optional<std::string>
createRole(const Vault::AppRole &appRoleAdmin) {
  Vault::Parameters parameters{{"token_policies", "example"}};
  return appRoleAdmin.create(Vault::Path{"example"}, parameters);
}

inline std::optional<std::string> createRole(const Vault::JwtOidc &jwtAdmin) {
  Vault::Parameters parameters{{"role_type", "jwt"},
                               {"user_claim", "example"},
                               {"bound_audiences", "example"},
                               {"policies", "example"}};

  return jwtAdmin.createRole(Vault::Path{"example"}, parameters);
}

inline std::optional<std::string>
deleteRole(const Vault::AppRole &appRoleAdmin) {
  return appRoleAdmin.del(Vault::Path{"example"});
}

inline std::optional<std::string> deleteRole(const Vault::JwtOidc &jwtAdmin) {
  return jwtAdmin.deleteRole(Vault::Path{"example"});
}

inline Vault::RoleId getRoleId(const Vault::AppRole &appRoleAdmin) {
  auto response = appRoleAdmin.getRoleId(Vault::Path{"example"});
  if (response) {
    return Vault::RoleId{
        nlohmann::json::parse(response.value())["data"]["role_id"]};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

inline Vault::SecretId getSecretId(const Vault::AppRole &appRoleAdmin) {
  auto response = appRoleAdmin.generateSecretId(Vault::Path{"example"},
                                                Vault::Parameters{});
  if (response) {
    return Vault::SecretId{
        nlohmann::json::parse(response.value())["data"]["secret_id"]};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

inline std::optional<std::string>
enableKeyValue(const Vault::Sys::Mounts &mountAdmin,
               const Vault::SecretMount &secretMount) {
  return mountAdmin.enable(secretMount, Vault::Parameters{{"type", "kv"}},
                           Vault::Parameters{}, Vault::Parameters{});
}

inline std::optional<std::string>
disableKeyValue(const Vault::Sys::Mounts &mountAdmin,
                const Vault::SecretMount &secretMount) {
  return mountAdmin.disable(secretMount);
}

inline std::optional<std::string>
enableJwtAuthentication(const Vault::Sys::Auth &authAdmin) {
  return authAdmin.enable(Vault::Path{"jwt"},
                          Vault::Parameters{{"type", "jwt"}});
}

inline std::optional<std::string>
disableJwtAuthentication(const Vault::Sys::Auth &authAdmin) {
  return authAdmin.disable(Vault::Path{"jwt"});
}

inline std::optional<std::string>
configureJwtAuthentication(const Vault::JwtOidc &jwtAdmin,
                           std::string publicKeyString) {
  Vault::Parameters parameters{{"jwt_validation_pubkeys", publicKeyString}};
  return jwtAdmin.configure(parameters);
}

inline std::optional<std::string>
enablePki(const Vault::Sys::Mounts &mountAdmin,
          const Vault::SecretMount &mount) {
  return mountAdmin.enable(mount, Vault::Parameters{{"type", "pki"}},
                           Vault::Parameters{}, Vault::Parameters{});
}

inline std::optional<std::string>
disablePki(const Vault::Sys::Mounts &mountAdmin,
           const Vault::SecretMount &mount) {
  return mountAdmin.disable(mount);
}
