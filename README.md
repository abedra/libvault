# libvault

This repository contains a C++ library for accessing the KV secret store inside of a [Hashicorp Vault Server](https://www.vaultproject.io/)

## Dependencies

* cURL

## Usage

The following example shows both a secret put and get. The most common scenario is get for most applications and the put will happen outside of the application by another process.

```cpp
#include <iostream>
#include "src/VaultClient.h"

auto main() -> int
{
    VaultClient vaultClient = VaultClient("localhost", "8200", "3b6122de-3948-8a16-55d3-322e8bab43d8", true);

    std::unordered_map<std::string, std::string> data(
    {
        {"db_user","user"},
        {"db_pass","pass"},
        {"api_key", "abc123"},
    });

    vaultClient.put("my_application", data);

    std::string getResponse = vaultClient.get("my_application");
    std::cout << getResponse << std::endl;

    return 0;
}
```

## JSON Serialization

This project uses [nlohmann/json](https://github.com/nlohmann/json) internally but does not expose it. This project makes no assumptions about serialization and returns `std:string` values that can be serialized by the tooling of your choice. Should you choose to use [nlohmann/json](https://github.com/nlohmann/json) you can add the `json.hpp` file to your project and turn the result of  `VaultClient::get()` into a usable structure using the following example:

```cpp
#include "json.hpp"

using json = nlohmann::json;

// additional code removed for brevity, see example above
auto data = json::parse(vaultClient.get("my_application");
std::cout << data["data"] << std::endl;
``` 

## Compile and Install

This project uses [CMake](https://cmake.org/). To build the library run the following commands from the project root:

```sh
cmake .
make
sudo make install
```