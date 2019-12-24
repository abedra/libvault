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
  AppRoleStrategy authStrategy{Vault::RoleId{"<role_id>"}, Vault::SecretId{"<secret_id>"}};
  VaultClient vaultClient{config, authStrategy, httpErrorCallback};

  KeyValue kv{vaultClient, KeyValue::Version::v1};
  Vault::Path mount{"/test"};
  KeyValue kv2{vaultClient, mount};

  Parameters parameters(
    {
      {"foo","world"},
      {"baz","quux"},
      {"something", "something else"},
    });

  Vault::Path key{"hello"};

  kv.put(key, parameters);
  kv2.put(key, parameters);

  std::cout << kv.get(key).value() << std::endl;
  std::cout << kv2.get(key).value() << std::endl;
}
```

## JSON Serialization

This project uses [nlohmann/json](https://github.com/nlohmann/json) internally but does not expose it. This project makes no assumptions about serialization and returns `std:string` values that can be serialized by the tooling of your choice. Should you choose to use [nlohmann/json](https://github.com/nlohmann/json) you can add the `json.hpp` file to your project. This projects integration tests have multiple examples of how to use it.


## Compile and Install

This project uses [CMake](https://cmake.org/). To build the library run the following commands from the project root:

```sh
mkdir build
cd build
cmake ../
make
```
