#include "VaultClient.h"

Vault::ConfigBuilder& Vault::ConfigBuilder::withTlsEnabled(bool flag) {
  config_.tls_ = flag;
  return *this;
}

Vault::ConfigBuilder&  Vault::ConfigBuilder::withDebug(bool flag) {
  config_.debug_ = flag;
  return *this;
}

Vault::ConfigBuilder& Vault::ConfigBuilder::withTlsVerification(bool flag) {
  config_.verify_ = flag;
  return *this;
}

Vault::ConfigBuilder& Vault::ConfigBuilder::withHost(Vault::Host host) {
  config_.host_ = std::move(host);
  return *this;
}

Vault::ConfigBuilder& Vault::ConfigBuilder::withPort(Vault::Port port) {
  config_.port_ = std::move(port);
  return *this;
}

Vault::ConfigBuilder& Vault::ConfigBuilder::withNamespace(Vault::Namespace ns) {
  config_.ns_ = std::move(ns);
  return *this;
}

Vault::ConfigBuilder& Vault::ConfigBuilder::withConnectTimeout(Vault::Timeout timeout) {
  config_.connectTimeout_ = timeout;
  return *this;
}

Vault::ConfigBuilder& Vault::ConfigBuilder::withRequestTimeout(Vault::Timeout timeout) {
  config_.requestTimeout_ = timeout;
  return *this;
}

Vault::ConfigBuilder& Vault::ConfigBuilder::withLowSpeedTimeout(Vault::Timeout timeout) {
  config_.lowSpeedTimeout_ = timeout;
  return *this;
}

Vault::ConfigBuilder& Vault::ConfigBuilder::withLowSpeedLimit(Vault::Threshold threshold) {
  config_.lowSpeedLimit_ = threshold;
  return *this;
}

Vault::ConfigBuilder& Vault::ConfigBuilder::withCaBundle(const std::filesystem::path &caBundle) {
  config_.caBundle_ = caBundle;
  return *this;
}

Vault::Config& Vault::ConfigBuilder::build() {
  return config_;
}
