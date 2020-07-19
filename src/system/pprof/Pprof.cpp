#include "VaultClient.h"

std::optional<std::string> Vault::Sys::Pprof::index() {
  return HttpConsumer::get(client_, getUrl(Path{}));
}

std::optional<std::string> Vault::Sys::Pprof::cmdline() {
  return HttpConsumer::get(client_, getUrl(Path{"cmdline"}));
}

std::optional<std::string> Vault::Sys::Pprof::goroutine() {
  return HttpConsumer::get(client_, getUrl(Path{"goroutine"}));
}

std::optional<std::string> Vault::Sys::Pprof::heap() {
  return HttpConsumer::get(client_, getUrl(Path{"heap"}));
}

std::optional<std::string> Vault::Sys::Pprof::profile() {
  return HttpConsumer::get(client_, getUrl(Path{"profile"}));
}

std::optional<std::string> Vault::Sys::Pprof::symbol() {
  return HttpConsumer::get(client_, getUrl(Path{"symbol"}));
}

std::optional<std::string> Vault::Sys::Pprof::trace() {
  return HttpConsumer::get(client_, getUrl(Path{"trace"}));
}

Vault::Url Vault::Sys::Pprof::getUrl(const Path &path) {
  return client_.getUrl("/v1/sys/pprof/", path);
}

