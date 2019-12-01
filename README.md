# libvault

This repository contains a C++ library for accessing the KV secret store inside of a [Hashicorp Vault Server](https://www.vaultproject.io/)

## Dependencies

* cURL

## Usage

The following example shows both a secret put and get. The most common scenario is get for most applications and the put will happen outside of the application by another process. Additional examples are located in the [example](example) directory.

```cpp
#include <iostream>
#include "VaultClient.h"

int main(void)
{
    HttpErrorCallback httpErrorCallback = [&](std::string err) {
      std::cout << err << std::endl;
    };

    auto config = VaultConfigBuilder().build();
    AppRoleStrategy authStrategy{RoleId{"<role_id>"}, SecretId{"<secret_id>"}};
    VaultClient vaultClient{config, authStrategy, httpErrorCallback};

    KeyValue kv{vaultClient, KeyValue::Version::v1};
    Path mount{"/test"};
    KeyValue kv2{vaultClient, mount};

    Parameters parameters(
    {
        {"foo","world"},
        {"baz","quux"},
        {"something", "something else"},
    });

    Path key{"hello"};

    kv.put(key, parameters);
    kv2.put(key, parameters);

    std::cout << kv.get(key) << std::endl;
    std::cout << kv2.get(key) << std::endl;
}
```

## JSON Serialization

This project uses [nlohmann/json](https://github.com/nlohmann/json) internally but does not expose it. This project makes no assumptions about serialization and returns `std:string` values that can be serialized by the tooling of your choice. Should you choose to use [nlohmann/json](https://github.com/nlohmann/json) you can add the `json.hpp` file to your project and turn the result of  `VaultClient::get()` into a usable structure using the following example:

```cpp
#include "json.hpp"

using json = nlohmann::json;

// additional code removed for brevity, see example above
auto data = json::parse(vaultClient.get("my_application"));
std::cout << data["data"] << std::endl;
```

## Compile and Install

This project uses [CMake](https://cmake.org/). To build the library run the following commands from the project root:

```sh
mkdir build
cd build
cmake ../
make
```
