#include <catch2/catch.hpp>
#include <optional>

#include "json.hpp"

#include "VaultClient.h"
#include "TestSetup.h"

static void setValues(KeyValue kv, Vault::Path path) {
  Parameters data(
      {
          {"foo","world"},
          {"baz","quux"},
          {"something", "something else"},
      });

  kv.put(path, data);
}

static void assertListValues(KeyValue kv) {
  // TODO: Should probably refactor the list method, this doesn't make much sense
  auto response = kv.list(Vault::Path{""});

  if (response) {
    std::vector<std::string> secrets = nlohmann::json::parse(response.value())["data"]["keys"];

    CHECK(secrets.size() == 1);
    CHECK(secrets.at(0) == "hello");
  } else {
    CHECK(false);
  }
}

static void assertGetValues(KeyValue kv, const Vault::Path& path) {
  auto response = kv.get(path);

  if (response) {
    std::unordered_map<std::string, std::string> secrets = nlohmann::json::parse(response.value())["data"]["data"];

    CHECK(secrets.size() == 3);

    auto baz = secrets.find("baz");
    CHECK(baz != secrets.end());
    CHECK(baz->second == "quux");
  } else {
    CHECK(false);
  }
}

static void assertDestroyValues(KeyValue kv, const Vault::Path& path) {
  kv.del(path);

  CHECK(!kv.get(path));
}

TEST_CASE("KeyValue Functions") {
  VaultClient vaultClient = login();

  SECTION("v2") {
    KeyValue kv(vaultClient);
    Vault::Path path("hello");

    setValues(kv, path);
    assertListValues(kv);
    assertGetValues(kv, path);
    assertDestroyValues(kv, path);
  }
}
