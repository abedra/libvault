# libvault
[![Build Status](https://travis-ci.org/abedra/libvault.svg?branch=master)](https://travis-ci.org/abedra/libvault)
<a href="https://lgtm.com/projects/g/abedra/libvault/">
   <img src="https://img.shields.io/lgtm/alerts/g/abedra/libvault" alt="Total alerts"/>
</a>
![LGTM Grade](https://img.shields.io/lgtm/grade/cpp/github/abedra/libvault)
[![Version](https://img.shields.io/badge/version-0.28.0-4a8fff)](https://img.shields.io/badge/version-0.28.0-4a8fff)

A C++ library for [Hashicorp Vault](https://www.vaultproject.io/)

## C++ 17

This project assumes a C++ 17 capable compiler. This includes GCC 8 or higher, and clang 3.8 or higher. Support for lower standards versions of C++ will not be accepted due to inconsistent implementations of optional.

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
  Vault::HttpErrorCallback httpErrorCallback = [&](std::string err) {
    std::cout << err << std::endl;
  };


  Vault::AppRoleStrategy authStrategy{
    Vault::RoleId{"<role_id>"},
    Vault::SecretId{"<secret_id>"}
  };

  Vault::Config config = Vault::ConfigBuilder().build();
  Vault::Client vaultClient{config, authStrategy, httpErrorCallback};
  Vault::KeyValue kv{vaultClient, Vault::KeyValue::Version::v1};
  Vault::Path mount{"/test"};
  Vault::KeyValue kv2{vaultClient, mount};
  Vault::Parameters parameters(
    {
      {"foo","world"},
      {"baz","quux"},
      {"something", "something else"},
    }
  );

  Vault::Path key{"hello"};

  kv.create(key, parameters);
  kv2.create(key, parameters);

  std::cout << kv.read(key).value() << std::endl;
  std::cout << kv2.read(key).value() << std::endl;
}
```

## JSON Serialization

This project uses [nlohmann/json](https://github.com/nlohmann/json) internally but does not expose it. This project makes no assumptions about serialization and returns `std:string` values that can be serialized by the tooling of your choice. Should you choose to use [nlohmann/json](https://github.com/nlohmann/json) you can add the `json.hpp` file to your project. This project's integration tests have multiple examples of how to use it.

## Feature Support

The following tables show support for each of the secret backends, auth methods, and system endpoints. Because the surface area is so large, endpoints are implemented as needed. Pull requests are welcome. Feel free to file an issue or submit a pull request for additional support.

| Secret Backend   | Implemented | Integration Tested |
|------------------|-------------|--------------------|
| TOTP             | Yes         | Yes                |
| Cubbyhole        | Yes         | Yes                |
| Key/Value V1     | Yes         | Partial            |
| Key/Value V2     | Yes         | Partial            |
| RabbitMQ         | Yes         | Partial            |
| Transit          | Yes         | Partial            |
| PKI              | Yes         | Partial            |
| PostgreSQL       | Yes         | Partial            |
| Cassandra        | Yes         | No                 |
| Elasticsearch    | Yes         | No                 |
| Influxdb         | Yes         | No                 |
| HanaDB           | Yes         | No                 |
| MSSQL            | Yes         | No                 |
| MySQL/MariaDB    | Yes         | No                 |
| Oracle           | Yes         | No                 |
| SSH              | Yes         | No                 |
| Active Directory | Yes         | No                 |
| Ali Cloud        | Yes         | No                 |
| AWS              | No          | No                 |
| Azure            | No          | No                 |
| Google Cloud     | No          | No                 |
| Google Cloud KMS | No          | No                 |
| KMIP             | No          | No*                |
| Identities       | No          | No                 |
| Nomad            | No          | No                 |
| Consul           | No          | No                 |
| MongoDB Atlas    | No          | No                 |
| OpenLDAP         | No          | No                 |
| Transform        | No          | No*                |

\* Requires an enterprise license and cannot be integration tested

| Auth Method       | Implemented | Integration Tested |
|-------------------|-------------|--------------------|
| AppRole           | Yes         | Yes                |
| LDAP              | Partial     | No                 |
| Tokens            | Partial     | Partial            |
| AliCloud          | Yes         | No                 |
| AWS               | No          | No                 |
| Azure             | No          | No                 |
| Cloud Foundry     | No          | No                 |
| GitHub            | No          | No                 |
| Google Cloud      | No          | No                 |
| JWT/OIDC          | No          | No                 |
| Kerberos          | No          | No                 |
| Kubernetes        | No          | No                 |
| OCI               | No          | No                 |
| Okta              | No          | No                 |
| RADIUS            | No          | No                 |
| TLS Certificate   | No          | No                 |
| Username/Password | No          | No                 |

\* Requires an enterprise license and cannot be integration tested

| System Backend              | Implemented | Integration Tested |
|-----------------------------|-------------|--------------------|
| /sys/health                 | Yes         | Yes                |
| /sys/leader                 | Yes         | Yes                |
| /sys/auth                   | Yes         | Yes                |
| /sys/wrapping               | Yes         | Yes                |
| /sys/audit                  | Yes         | No                 |
| /sys/audit-hash             | Yes         | No                 |
| /sys/capabilities           | Yes         | No                 |
| /sys/capabilities-accessor  | Yes         | No                 |
| /sys/capabilities-self      | Yes         | No                 |
| /sys/config/auditing        | Yes         | No                 |
| /sys/config/control-group   | Yes         | No                 |
| /sys/config/cors            | Yes         | No                 |
| /sys/config/state           | Yes         | No                 |
| /sys/config/ui              | Yes         | No                 |
| /sys/control-group          | Yes         | No                 |
| /sys/host-info              | Yes         | No                 |
| /sys/init                   | Yes         | No                 |
| /sys/internal/counters      | Yes         | No                 |
| /sys/plugins/reload/backend | Yes         | No                 |
| /sys/internal/ui/mounts     | Yes         | No                 |
| /sys/key-status             | Yes         | No                 |
| /sys/leases                 | Yes         | No                 |
| /sys/license                | Yes         | No                 |
| /sys/metrics                | Yes         | No                 |
| /sys/mounts                 | Yes         | No                 |
| /sys/remount                | Yes         | No                 |
| /sys/rotate                 | Yes         | No                 |
| /sys/seal                   | Yes         | No                 |
| /sys/seal-status            | Yes         | No                 |
| /sys/step-down              | Yes         | No                 |
| /sys/unseal                 | Yes         | No                 |
| /sys/mfa                    | Yes         | No                 |
| /sys/namespaces             | Yes         | No*                |
| /sys/generate-root          | Yes         | No                 |
| /sys/internal/specs/openapi | Yes         | No                 |
| /sys/plugins/catalog        | Yes         | No                 |
| /sys/policy                 | Yes         | No                 |
| /sys/policies               | Yes         | No*                |
| /sys/pprof                  | Yes         | No                 |
| /sys/raw                    | Yes         | No                 |
| /sys/sealwrap/rewrap        | Yes         | No                 |
| /sys/tools                  | Yes         | No                 |
| /sys/rekey                  | No          | No                 |
| /sys/rekey-recovery-key     | No          | No                 |
| /sys/replication            | No          | No                 |
| /sys/storage                | No          | No                 |

\* Requires an enterprise license and cannot be integration tested

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
* `ENABLE_INTEGRATION_TEST [ON|OFF]` - `[Default OFF]` Enable integration tests (Requires configured, running Vault)
* `ENABLE_COVERAGE [ON|OFF]` - `[Default OFF]` Enable gcov code coverage
* `LINK_CURL [ON|OFF]` - `[Default OFF]` Link curl library when compiling

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
