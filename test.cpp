#include <iostream>
#include "src/VaultClient.h"

auto main() -> int
{
    VaultClient vaultClient = VaultClient("localhost", "8200", "3b6122de-3948-8a16-55d3-322e8bab43d8", true);

    std::unordered_map<std::string, std::string> data(
    {
        {"foo","world"},
        {"baz","quux"},
        {"something", "something else"},
    });

    vaultClient.put("hello", data);

    std::string getResponse = vaultClient.get("hello");
    std::cout << getResponse << std::endl;

    return 0;
}