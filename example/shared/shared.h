#pragma once
#include <iostream>
#include "../../lib/json.hpp"
#include "libvault/VaultClient.h"

Vault::Client getRootClient(const Vault::Token &rootToken) {
  Vault::TokenStrategy tokenStrategy{rootToken};
  Vault::Config config = Vault::ConfigBuilder().withDebug(false).withTlsEnabled(false).build();
  Vault::HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };
  Vault::ResponseErrorCallback responseCallback = [&](Vault::HttpResponse err) {
    std::cout << err.statusCode << " : " << err.url.value() << " : " << err.body.value() << std::endl;
  };
  return Vault::Client{config, tokenStrategy, httpErrorCallback, responseCallback};
}

Vault::Client getAppRoleClient(const Vault::RoleId &roleId, const Vault::SecretId &secretId) {
  Vault::AppRoleStrategy authStrategy{roleId, secretId};
  Vault::Config config = Vault::ConfigBuilder().withTlsEnabled(false).build();

  return Vault::Client{config, authStrategy};
}

Vault::Client getJwtClient(const Vault::RoleId &role, const Vault::Jwt &jwt) {
  Vault::JwtStrategy authStrategy{role, jwt};
  Vault::Config config = Vault::ConfigBuilder().withDebug(false).withTlsEnabled(false).build();
  Vault::HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };
  Vault::ResponseErrorCallback responseCallback = [&](Vault::HttpResponse err) {
    std::cout << err.statusCode << " : " << err.url.value() << " : " << err.body.value() << std::endl;
  };

  return Vault::Client{config, authStrategy, httpErrorCallback, responseCallback};
}

std::optional<std::string> createPolicy(const Vault::Sys::Policy &policyAdmin) {
  Vault::Parameters parameters{{
                                   "policy", "path \"secret/*\" {capabilities = [\"read\", \"update\", \"list\", \"delete\", \"create\"]}"
                               }};
  return policyAdmin.create(Vault::Path{"example"}, parameters);
}

std::optional<std::string> deletePolicy(const Vault::Sys::Policy &policyAdmin) {
  return policyAdmin.del(Vault::Path{"example"});
}

std::optional<std::string> enableAppRole(const Vault::Sys::Auth &authAdmin) {
  return authAdmin.enable(Vault::Path{"approle"}, Vault::Parameters{{"type", "approle"}});
}

std::optional<std::string> disableAppRole(const Vault::Sys::Auth &authAdmin) {
  return authAdmin.disable(Vault::Path{"approle"});
}

std::optional<std::string> createRole(const Vault::AppRole &appRoleAdmin) {
  Vault::Parameters parameters{{"token_policies", "example"}};
  return appRoleAdmin.create(Vault::Path{"example"}, parameters);
}

std::optional<std::string> createRole(const Vault::JwtOidc &jwtAdmin) {
  Vault::Parameters  parameters{
    {"role_type", "jwt"},
    {"user_claim", "example"},
    {"bound_audiences", "example"},
    {"policies", "example"}
  };

  return jwtAdmin.createRole(Vault::Path{"example"}, parameters);
}

std::optional<std::string> deleteRole(const Vault::AppRole &appRoleAdmin) {
  return appRoleAdmin.del(Vault::Path{"example"});
}

std::optional<std::string> deleteRole(const Vault::JwtOidc &jwtAdmin) {
  return jwtAdmin.deleteRole(Vault::Path{"example"});
}

Vault::RoleId getRoleId(const Vault::AppRole &appRoleAdmin) {
  auto response = appRoleAdmin.getRoleId(Vault::Path{"example"});
  if (response) {
    return Vault::RoleId{nlohmann::json::parse(response.value())["data"]["role_id"]};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

Vault::SecretId getSecretId(const Vault::AppRole &appRoleAdmin) {
  auto response = appRoleAdmin.generateSecretId(Vault::Path{"example"}, Vault::Parameters{});
  if (response) {
    return Vault::SecretId{nlohmann::json::parse(response.value())["data"]["secret_id"]};
  } else {
    std::cout << "Could not get role id" << std::endl;
    exit(-1);
  }
}

std::optional<std::string> enableKeyValue(const Vault::Sys::Mounts &mountAdmin) {
  return mountAdmin.enable(Vault::Path{}, Vault::Parameters{}, Vault::Parameters{}, Vault::Parameters{});
}

std::optional<std::string> disableKeyValue(const Vault::Sys::Mounts &mountAdmin) {
  return mountAdmin.disable(Vault::Path{});
}

std::optional<std::string> enableJwtAuthentication(const Vault::Sys::Auth &authAdmin) {
  return authAdmin.enable(Vault::Path{"jwt"}, Vault::Parameters{{"type", "jwt"}});
}

std::optional<std::string> disableJwtAuthentication(const Vault::Sys::Auth &authAdmin) {
  return authAdmin.disable(Vault::Path{"jwt"});
}

std::optional<std::string> configureJwtAuthentication(const Vault::JwtOidc &jwtAdmin, std::string publicKeyString) {
  Vault::Parameters parameters{{"jwt_validation_pubkeys", publicKeyString}};
  return jwtAdmin.configure(parameters);
}