#include "VaultClient.h"

VaultConfigBuilder& VaultConfigBuilder::withTlsEnabled(bool flag) {
  config_.tls_ = flag;
  return *this;
}

VaultConfigBuilder&  VaultConfigBuilder::withDebug(bool flag) {
  config_.debug_ = flag;
  return *this;
}

VaultConfigBuilder& VaultConfigBuilder::withTlsVerification(bool flag) {
  config_.verify_ = flag;
  return *this;
}

VaultConfigBuilder& VaultConfigBuilder::withHost(VaultHost host) {
  config_.host_ = std::move(host);
  return *this;
}

VaultConfigBuilder& VaultConfigBuilder::withPort(VaultPort port) {
  config_.port_ = std::move(port);
  return *this;
}

VaultConfigBuilder& VaultConfigBuilder::withNamespace(Namespace ns) {
  config_.ns_ = std::move(ns);
  return *this;
}

VaultConfigBuilder& VaultConfigBuilder::withConnectTimeout(VaultConnectTimeout timeout) {
  config_.connectTimeout_ = timeout;
  return *this;
}

VaultConfig& VaultConfigBuilder::build() {
  return config_;
}
