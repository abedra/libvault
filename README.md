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
    auto authStrategy = AppRole{"<role_id>", "<secret_id>"};
    auto vaultClient = VaultClient{config, authStrategy, httpErrorCallback};

    auto kv = KeyValue(vaultClient, KeyValue::Version::v1);
    auto kv2 = KeyValue(vaultClient, "/test");

    Parameters parameters(
    {
        {"foo","world"},
        {"baz","quux"},
        {"something", "something else"},
    });

    kv.put("hello", parameters);
    kv2.put("hello", parameters);

    std::cout << kv.get("hello") << std::endl;
    std::cout << kv2.get("hello") << std::endl;
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

### windows VC14 (Visual Studio 2015) or greater
```
mkdir build && cd build
cmake -G "Visual Studio 14 2015 Win64" .. -DCURL_INCLUDE_DIR="C:\path\to\curl\include" -DCURL_LIBRARY="C:\path\to\curl\binary\libcurl.lib" -DENABLE_TEST=OFF -DSHARED_LIB=ON
open vault.sln and build

```
