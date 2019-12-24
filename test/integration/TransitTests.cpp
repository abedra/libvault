#include <catch2/catch.hpp>

#include "VaultClient.h"
#include "TestHelpers.h"

TEST_CASE("Transit Functions")
{
  VaultClient vaultClient = TestHelpers::AppRole::login();
  Transit transit(vaultClient);
  Vault::Path path("mykey");

  SECTION("Encrypt/Decrypt Round Trip")
  {
    Parameters plaintext({ {"plaintext", Base64::encode("Attack at dawn")} });
    auto encryptResponse = transit.encrypt(path, plaintext);

    if (encryptResponse) {
      std::string ciphertextString = nlohmann::json::parse(encryptResponse.value())["data"]["ciphertext"];
      Parameters ciphertext({ {"ciphertext", ciphertextString} });
      auto decryptResponse = transit.decrypt(path, ciphertext);

      if (decryptResponse) {
        CHECK(decryptResponse.value() == "Attack at dawn");
      } else {
        CHECK(false);
      }
    } else {
      CHECK(false);
    }
  }

  SECTION("Generate Data Key") {
    auto response = transit.generate_data_key(path, {{}});

    if (response) {
      std::string key = nlohmann::json::parse(response.value())["data"]["plaintext"];

      CHECK(key.length() == 44);
    } else {
      CHECK(false);
    }
  }

  SECTION("Generate Wrapped Data Key")
  {
    auto response = transit.generate_wrapped_data_key(path, {{}});

    if (response) {
      std::string key = nlohmann::json::parse(response.value())["data"]["ciphertext"];

      CHECK(key.length() == 89);
    } else {
      CHECK(false);
    }
  }
}
