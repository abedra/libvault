#include <catch2/catch.hpp>

#include "VaultClient.h"
#include "../TestHelpers.h"

TEST_CASE("Transit Functions")
{
  Vault::Client vaultClient = TestHelpers::AppRole::login();
  Vault::Transit transit(vaultClient);
  Vault::Path path("mykey");

  SECTION("Encrypt/Decrypt Round Trip")
  {
    Vault::Parameters plaintext({ {"plaintext", Vault::Base64::encode("Attack at dawn")} });
    auto encryptResponse = transit.encrypt(path, plaintext);

    if (encryptResponse) {
      std::string ciphertextString = nlohmann::json::parse(encryptResponse.value())["data"]["ciphertext"];
      Vault::Parameters ciphertext({ {"ciphertext", ciphertextString} });
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
    auto response = transit.generateDataKey(path, {{}});

    if (response) {
      std::string key = nlohmann::json::parse(response.value())["data"]["plaintext"];

      CHECK(key.length() == 44);
    } else {
      CHECK(false);
    }
  }

  SECTION("Generate Wrapped Data Key")
  {
    auto response = transit.generateWrappedDataKey(path, {{}});

    if (response) {
      std::string key = nlohmann::json::parse(response.value())["data"]["ciphertext"];

      CHECK(key.length() == 89);
    } else {
      CHECK(false);
    }
  }

  SECTION("Random base64 bytes")
  {
    Vault::Parameters parameters({ {"format","base64"} });
    auto response = transit.generateRandomBytes(32, parameters);

    if (response) {
      std::string data = nlohmann::json::parse(response.value())["data"]["random_bytes"];

      CHECK(data.length() == 44);
    } else {
      CHECK(false);
    }
  }

  SECTION("Random hex bytes")
  {
    Vault::Parameters parameters({ {"format","hex"} });
    auto response = transit.generateRandomBytes(32, parameters);

    if (response) {
      std::string data = nlohmann::json::parse(response.value())["data"]["random_bytes"];

      CHECK(data.length() == 64);
    } else {
      CHECK(false);
    }
  }

  SECTION("Hash base64") {
    auto input = Vault::Base64::encode("Attack at dawn");
    Vault::Parameters parameters({ {"format","base64"}, {"input", input} });

    auto sha224 = transit.hash(Vault::Algorithms::SHA2_224, parameters);
    auto sha256 = transit.hash(Vault::Algorithms::SHA2_256, parameters);
    auto sha384 = transit.hash(Vault::Algorithms::SHA2_384, parameters);
    auto sha512 = transit.hash(Vault::Algorithms::SHA2_512, parameters);

    TestHelpers::Transit::assertHashPopulated(sha224);
    TestHelpers::Transit::assertHashPopulated(sha256);
    TestHelpers::Transit::assertHashPopulated(sha384);
    TestHelpers::Transit::assertHashPopulated(sha512);
  }

  SECTION("Hash hex")
  {
    auto input = Vault::Base64::encode("Attack at dawn");
    Vault::Parameters parameters({ {"format","hex"}, {"input", input} });

    auto sha224 = transit.hash(Vault::Algorithms::SHA2_224, parameters);
    auto sha256 = transit.hash(Vault::Algorithms::SHA2_256, parameters);
    auto sha384 = transit.hash(Vault::Algorithms::SHA2_384, parameters);
    auto sha512 = transit.hash(Vault::Algorithms::SHA2_512, parameters);

    TestHelpers::Transit::assertHashPopulated(sha224);
    TestHelpers::Transit::assertHashPopulated(sha256);
    TestHelpers::Transit::assertHashPopulated(sha384);
    TestHelpers::Transit::assertHashPopulated(sha512);
  }

  SECTION("HMac")
  {
    Vault::Parameters parameters({ {"input", Vault::Base64::encode("Attack at dawn")} });

    auto sha224 = transit.hmac(path, Vault::Algorithms::SHA2_224, parameters);
    auto sha256 = transit.hmac(path, Vault::Algorithms::SHA2_256, parameters);
    auto sha384 = transit.hmac(path, Vault::Algorithms::SHA2_384, parameters);
    auto sha512 = transit.hmac(path, Vault::Algorithms::SHA2_512, parameters);

    TestHelpers::Transit::assertHmacPopulated(sha224);
    TestHelpers::Transit::assertHmacPopulated(sha256);
    TestHelpers::Transit::assertHmacPopulated(sha384);
    TestHelpers::Transit::assertHmacPopulated(sha512);
  }

  SECTION("Sign")
  {
    Vault::Path signkey("signkey");
    Vault::Parameters parameters({ {"input", Vault::Base64::encode("Attack at dawn")} });

    auto sha1 = transit.sign(signkey, Vault::Algorithms::SHA1, parameters);
    auto sha224 = transit.sign(signkey, Vault::Algorithms::SHA2_224, parameters);
    auto sha256 = transit.sign(signkey, Vault::Algorithms::SHA2_256, parameters);
    auto sha384 = transit.sign(signkey, Vault::Algorithms::SHA2_384, parameters);
    auto sha512 = transit.sign(signkey, Vault::Algorithms::SHA2_512, parameters);

    TestHelpers::Transit::assertSignaturePopulated(sha1);
    TestHelpers::Transit::assertSignaturePopulated(sha224);
    TestHelpers::Transit::assertSignaturePopulated(sha256);
    TestHelpers::Transit::assertSignaturePopulated(sha384);
    TestHelpers::Transit::assertSignaturePopulated(sha512);
  }
}
