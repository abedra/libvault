#include "VaultClient.h"

VaultClient::VaultClient(std::string host, std::string port, AppRole& appRole) :
  host_(host), port_(port), appRole_(appRole) {
  this->httpClient_ = HttpClient(false);
  token_ = appRole_.authenticate(this);
}

VaultClient::VaultClient(std::string host, std::string port, AppRole& appRole, bool debug) :
  host_(host), port_(port), appRole_(appRole) {
  this->httpClient_ = HttpClient(debug);
  token_ = appRole_.authenticate(this);
}

std::string VaultClient::getUrl(std::string base, std::string path) {
  return "http://" + host_ + ":" + port_ + base + path;
}
