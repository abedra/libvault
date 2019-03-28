#include <iostream>
#include "VaultClient.h"

auto main() -> int
{
    auto authStrategy = AppRole("9ce0eddc-0cd5-dd87-4c08-eb5ee9b3eca6", "043f002e-de24-6cd0-a37c-d44601400fb1");
    auto vaultClient = VaultClient("127.0.0.1", "8200", authStrategy);
    auto kv = KeyValue(vaultClient, KeyValue::Version::v1);
    auto kv2 = KeyValue(vaultClient, "/test");

    std::unordered_map<std::string, std::string> data(
    {
        {"foo","world"},
        {"baz","quux"},
        {"something", "something else"},
    });

    kv.put("hello", data);
    kv2.put("hello", data);

    std::cout << kv.get("hello") << std::endl;
    std::cout << kv2.get("hello") << std::endl;
}
