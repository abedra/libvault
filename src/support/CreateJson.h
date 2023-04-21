#pragma once
#include "VaultClient.h"
#include "json.hpp"
#include <string>
#include <vector>

namespace helpers {

inline nlohmann::json create_json(const Vault::Parameters &parameters) {
  nlohmann::json json = nlohmann::json::object();
  for (auto &[key, value] : parameters) {
    if (std::holds_alternative<std::string>(value)) {
      json[key] = std::get<std::string>(value);
    } else if (std::holds_alternative<int>(value)) {
      json[key] = std::get<int>(value);
    } else if (std::holds_alternative<std::vector<std::string>>(value)) {
      json[key] = std::get<std::vector<std::string>>(value);
    } else if (std::holds_alternative<Vault::Map>(value)) {
      auto map = std::get<Vault::Map>(value);
      nlohmann::json j;
      for (auto &[map_key, map_value] : map) {
        j[map_key] = map_value;
      }
      json[key] = j;
    }
  }
  return json;
}

} // namespace helpers
