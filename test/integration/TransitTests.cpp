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

  SECTION("Random base64 bytes")
  {
    Parameters parameters({ {"format","base64"} });
    auto response = transit.generate_random_bytes(32, parameters);

    if (response) {
      std::string data = nlohmann::json::parse(response.value())["data"]["random_bytes"];

      CHECK(data.length() == 44);
    } else {
      CHECK(false);
    }
  }

  SECTION("Random hex bytes")
  {
    Parameters parameters({ {"format","hex"} });
    auto response = transit.generate_random_bytes(32, parameters);

    if (response) {
      std::string data = nlohmann::json::parse(response.value())["data"]["random_bytes"];

      CHECK(data.length() == 64);
    } else {
      CHECK(false);
    }
  }

  SECTION("Hash base64") {
    auto input = Base64::encode("Attack at dawn");
    Parameters parameters({ {"format","base64"}, {"input", input} });

    auto sha224 = transit.hash(Vault::Algorithms::SHA2_224, parameters);
    auto sha256 = transit.hash(Vault::Algorithms::SHA2_256, parameters);
    auto sha384 = transit.hash(Vault::Algorithms::SHA2_384, parameters);
    auto sha512 = transit.hash(Vault::Algorithms::SHA2_512, parameters);

    TestHelpers::Transit::assertHashEquals(sha224, "+vs7sVxlLrvkDYfSbd7LyHKFBvnQyzPbZKj69Q==");
    TestHelpers::Transit::assertHashEquals(sha256, "+kPuU3On821JG+nldhfq4YrWLz+F1c7veO45/gDOXus=");
    TestHelpers::Transit::assertHashEquals(sha384, "d9lhlgQISkir4HQrRiRqAIofrlYOC4RwM9iiofd2209cpJK5Sg/TNf9K5Cok/JCm");
    TestHelpers::Transit::assertHashEquals(sha512, "MQNINod3m5cU1Xc8Gu5t/RUWUQ26i2OwA0w4JTFTmvW4NtopzpLGkCGsNM/6d2OMjrJd+EqsnBsIK+6t0niRSA==");
  }

  SECTION("Hash hex")
  {
    auto input = Base64::encode("Attack at dawn");
    Parameters parameters({ {"format","hex"}, {"input", input} });

    auto sha224 = transit.hash(Vault::Algorithms::SHA2_224, parameters);
    auto sha256 = transit.hash(Vault::Algorithms::SHA2_256, parameters);
    auto sha384 = transit.hash(Vault::Algorithms::SHA2_384, parameters);
    auto sha512 = transit.hash(Vault::Algorithms::SHA2_512, parameters);

    TestHelpers::Transit::assertHashEquals(sha224, "fafb3bb15c652ebbe40d87d26ddecbc8728506f9d0cb33db64a8faf5");
    TestHelpers::Transit::assertHashEquals(sha256, "fa43ee5373a7f36d491be9e57617eae18ad62f3f85d5ceef78ee39fe00ce5eeb");
    TestHelpers::Transit::assertHashEquals(sha384, "77d9619604084a48abe0742b46246a008a1fae560e0b847033d8a2a1f776db4f5ca492b94a0fd335ff4ae42a24fc90a6");
    TestHelpers::Transit::assertHashEquals(sha512, "3103483687779b9714d5773c1aee6dfd1516510dba8b63b0034c382531539af5b836da29ce92c69021ac34cffa77638c8eb25df84aac9c1b082beeadd2789148");
  }

  SECTION("HMac")
  {
    Parameters parameters({ {"input", Base64::encode("Attack at dawn")} });

    auto sha224 = transit.hmac(path, Vault::Algorithms::SHA2_224, parameters);
    auto sha256 = transit.hmac(path, Vault::Algorithms::SHA2_256, parameters);
    auto sha384 = transit.hmac(path, Vault::Algorithms::SHA2_384, parameters);
    auto sha512 = transit.hmac(path, Vault::Algorithms::SHA2_512, parameters);

    TestHelpers::Transit::assertHmacEquals(sha224, "vault:v1:kAM8jwMxlIPqsvM2adCAn8bNpr0mtd7EraoAoQ==");
    TestHelpers::Transit::assertHmacEquals(sha256, "vault:v1:aos50YYSKdGY9VfzwzLK4AkoTl5QREII17W44ntSlCg=");
    TestHelpers::Transit::assertHmacEquals(sha384, "vault:v1:+mw+T37535icD2XhAjAIETgRb7A30vy1UJnRv6uo0BDPhSeiUoERENLss6m3JZq3");
    TestHelpers::Transit::assertHmacEquals(sha512, "vault:v1:Iwss677l4ZoS1dtOjBwvwq5s044Mm6hVRSnjkVQOwdmX5hZJJVic7FcxOa0xP85ZhzzJczdSXRB/bnKsrwy5fA==");
  }
}
