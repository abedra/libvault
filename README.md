# libvault
[![Build Status](https://travis-ci.org/abedra/libvault.svg?branch=master)](https://travis-ci.org/abedra/libvault)
<a href="https://lgtm.com/projects/g/abedra/libvault/">
   <img src="https://img.shields.io/lgtm/alerts/g/abedra/libvault" alt="Total alerts"/>
</a>
![LGTM Grade](https://img.shields.io/lgtm/grade/cpp/github/abedra/libvault)
[![Version](https://img.shields.io/badge/version-0.4.2-4a8fff)](https://img.shields.io/badge/version-0.4.2-4a8fff)

A C++ library for [Hashicorp Vault](https://www.vaultproject.io/)

## C++ 17

This project assumes a C++ 17 capable compiler. This includes GCC 7 or higher, and clang 3.8 or higher. Support for lower standards versions of C++ will not be accepted due to inconsistent implementations of optional.

## Dependencies

* cURL
* Catch2 (test only)

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

This project uses [nlohmann/json](https://github.com/nlohmann/json) internally but does not expose it. This project makes no assumptions about serialization and returns `std:string` values that can be serialized by the tooling of your choice. Should you choose to use [nlohmann/json](https://github.com/nlohmann/json) you can add the `json.hpp` file to your project. This project's integration tests have multiple examples of how to use it.


## Compile and Install

This project uses [CMake](https://cmake.org/). To build the library run the following commands from the project root:

```sh
mkdir build
cd build
cmake ../
make
```

### Compile Options

The following custom options can be provided to CMake to control your build:

* `ENABLE_TEST [ON|OFF]` - `[Default ON]` Standard unit tests (Requires the Catch2 testing library)
* `ENABLE_INTEGRATION_TEST [ON|OFF]` - `[Default OFF]`Enable integration tests (Requires configured, running Vault)

## Local Development

This project uses a standard C++ development with CMake environment. Additionally, a running and configured instance of Vault is required to run the integration tests. This project contains scripts that will download Vault, configure it, and run it. You are of course welcome to use your own Vault instance, but you will need to use the configuration from the `script` folder for the tests to pass.

### Vault Setup

Use the provided scripts to setup and run your Vault environment:

```shell script
$ script/fetch_vault
$ script/vault
# In another terminal window
$ script/bootstrap
```

This will ensure you have a working instance of Vault that will work with the integration tests. Note that this setup does not demonstrate a production worthy configuration and should only be used for reference or inside of this project. For you production Vault setup please consult the Hashicorp Vault best practices.
