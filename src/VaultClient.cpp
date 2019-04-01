#include "VaultClient.h"

VaultClient::VaultClient(std::string host,
			 std::string port,
			 AuthenticationStrategy& authStrategy) :
  host_(host), port_(port), authStrategy_(authStrategy) {
  this->httpClient_ = HttpClient(false);
  auto result = authStrategy_.authenticate(this);
  if (result) {
    token_ = result.value();
  }
}

VaultClient::VaultClient(std::string host,
			 std::string port,
			 AuthenticationStrategy& authStrategy,
			 bool debug) :
  host_(host), port_(port), authStrategy_(authStrategy) {
  this->httpClient_ = HttpClient(debug);
  auto result = authStrategy_.authenticate(this);
  if (result) {
    token_ = result.value();
  }
}

std::string VaultClient::getUrl(std::string base, std::string path) {
  return "http://" + host_ + ":" + port_ + base + path;
}
