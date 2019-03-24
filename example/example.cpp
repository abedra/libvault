#include <iostream>
#include "VaultClient.h"

auto main() -> int
{
    VaultClient vaultClient = VaultClient("localhost", "8200", "s.P8ScpzeiJPp3YcwhA6ja3UpO");

    std::unordered_map<std::string, std::string> data(
    {
        {"foo","world"},
        {"baz","quux"},
        {"something", "something else"},
    });

    vaultClient.put("hello", data);

    std::string getResponse = vaultClient.get("hello");
    std::cout << getResponse << std::endl;

    vaultClient.del("hello");

    return 0;
}
