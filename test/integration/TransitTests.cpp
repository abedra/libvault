#include <catch2/catch.hpp>

#include "VaultClient.h"
#include "TestHelpers.h"

TEST_CASE("Transit Functions")
{
  VaultClient vaultClient = TestHelpers::AppRole::login();

  SECTION("Encrypt/Decrypt Round Trip")
  {
    Transit transit(vaultClient);
    Parameters plaintext({ {"plaintext", Base64::encode("Attack at dawn")} });
    Vault::Path path("mykey");
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
}
