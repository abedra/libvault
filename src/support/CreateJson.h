#pragma once
#include "json.hpp"
#include "VaultClient.h"

namespace helpers {

inline nlohmann::json create_json(const Vault::Parameters& parameters) {
  nlohmann::json json = nlohmann::json::object();
  for (auto&[key, value] : parameters) {
    if (std::holds_alternative<std::string>(value)) {
      json[key] = std::get<std::string>(value);
    }
    else if (std::holds_alternative<int>(value)) {
      json[key] = std::get<int>(value);
    }
    else if (std::holds_alternative<std::vector<std::string>>(value)) {
      json[key] = std::get<std::vector<std::string>>(value);
    }
  }
  return json;
}

}  // namespace helpers
