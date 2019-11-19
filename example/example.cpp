#include <iostream>
#include "VaultClient.h"

void print_response(std::experimental::optional<std::string> response) {
  if (response) {
    std::cout << response.value() << std::endl;
  } else {
    std::cout << "Error" << std::endl;
  }
}

void kv1(VaultClient vaultClient) {
  auto kv = KeyValue(vaultClient, KeyValue::Version::v1);

  std::unordered_map<std::string, std::string> data(
  {
    {"foo","bar"},
    {"baz","foo"},
    {"something", "quux"},
  });

  kv.put("hello", data);

  print_response(kv.get("hello"));
  print_response(kv.list("hello"));

  kv.del("hello");
  print_response(kv.get("hello"));
}

void kv2(VaultClient vaultClient) {
  auto kv = KeyValue(vaultClient, "test");

  std::unordered_map<std::string, std::string> data(
  {
    {"foo","world"},
    {"baz","quux"},
    {"something", "something else"},
  });

  kv.put("hello", data);

  print_response(kv.get("hello"));
  print_response(kv.list("hello"));

  kv.destroy("hello", std::vector<long>({40,41,42,43}));

  print_response(kv.get("hello"));
}

void transit_encrypt_decrypt(VaultClient vaultClient) {
  auto transit = Transit(vaultClient);

  auto input = Base64::encode("Attack at dawn");
  Parameters parameters({ {"plaintext", input} });
  print_response(transit.encrypt("mykey", parameters));

  parameters = Parameters({ {"ciphertext", "vault:v1:wOWt0eYKlzLwVKitJchP9F456jMtiFZUc/tC8+0l5BE2SJLVw548yy6W"} });
  print_response(transit.decrypt("mykey", parameters));
}

void transit_keys(VaultClient vaultClient) {
  auto transit = Transit(vaultClient);

  print_response(transit.generate_data_key("mykey", {{}}));
  print_response(transit.generate_wrapped_data_key("mykey", {{}}));
}

void transit_random(VaultClient vaultClient) {
  Transit transit(vaultClient);

  Parameters parameters({ {"format","base64"} });
  print_response(transit.generate_random_bytes(32, parameters));

  parameters = Parameters({ {"format","hex"} });
  print_response(transit.generate_random_bytes(32, parameters));
}

void transit_hash(VaultClient vaultClient) {
  auto sha_224 = "sha2-224";
  auto sha_256 = "sha2-256";
  auto sha_384 = "sha2-384";
  auto sha_512 = "sha2-512";

  Transit transit(vaultClient);

  auto input = Base64::encode("Attack at dawn");

  Parameters parameters({ {"format","base64"}, {"input", input} });

  print_response(transit.hash(sha_224, parameters));
  print_response(transit.hash(sha_256, parameters));
  print_response(transit.hash(sha_384, parameters));
  print_response(transit.hash(sha_512, parameters));

  parameters = Parameters({ {"format","hex"}, {"input", input} });

  print_response(transit.hash(sha_224, parameters));
  print_response(transit.hash(sha_256, parameters));
  print_response(transit.hash(sha_384, parameters));
  print_response(transit.hash(sha_512, parameters));
}

void transit_hmac(VaultClient vaultClient) {
  auto sha_224 = "sha2-224";
  auto sha_256 = "sha2-256";
  auto sha_384 = "sha2-384";
  auto sha_512 = "sha2-512";

  Transit transit(vaultClient);

  auto input = Base64::encode("Attack at dawn");

  Parameters parameters({ {"input", input} });

  print_response(transit.hmac("mykey", sha_224, parameters));
  print_response(transit.hmac("mykey", sha_256, parameters));
  print_response(transit.hmac("mykey", sha_384, parameters));
  print_response(transit.hmac("mykey", sha_512, parameters));
}

void transit_sign(VaultClient vaultClient) {
  auto sha_1   = "sha1";
  auto sha_224 = "sha2-224";
  auto sha_256 = "sha2-256";
  auto sha_384 = "sha2-384";
  auto sha_512 = "sha2-512";

  Transit transit(vaultClient);

  auto input = Base64::encode("Attack at dawn");

  Parameters parameters({ {"input", input} });

  print_response(transit.sign("mykey", sha_1, parameters));
  print_response(transit.sign("mykey", sha_224, parameters));
  print_response(transit.sign("mykey", sha_256, parameters));
  print_response(transit.sign("mykey", sha_384, parameters));
  print_response(transit.sign("mykey", sha_512, parameters));
}

static std::string getOrDefault(const char *name, std::string defaultValue) {
    auto value = std::getenv(name);

    return value ? value : defaultValue;
}

auto main() -> int {
    HttpErrorCallback httpErrorCallback = [&](std::string err) {
        std::cout << err << std::endl;
    };

    auto roleId = getOrDefault("APPROLE_ROLE_ID", "");
    auto secretId = getOrDefault("APPROLE_SECRET_ID", "");

    auto config = VaultConfig::make().host("192.168.1.20").tls(false).getConfig();
    auto authStrategy = AppRole(roleId, secretId);
    auto vaultClient = VaultClient(config, authStrategy, httpErrorCallback);

    kv1(vaultClient);
    kv2(vaultClient);
    transit_encrypt_decrypt(vaultClient);
    transit_keys(vaultClient);
    transit_random(vaultClient);
    transit_hash(vaultClient);
    transit_hmac(vaultClient);
}
