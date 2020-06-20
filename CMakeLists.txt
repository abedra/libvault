cmake_minimum_required(VERSION 3.9)
project(vault VERSION 0.25.0 DESCRIPTION "Vault library for C++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG --coverage)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

option(ENABLE_TEST "Enable tests?" ON)
option(ENABLE_INTEGRATION_TEST "Enable integration tests?" OFF)
option(ENABLE_COVERAGE "Enable gcov code coverage" OFF)
option(LINK_CURL "Link curl library for vault" OFF)

find_package(CURL)
if (CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIR})
else (CURL_FOUND)
  message(FATAL_ERROR "CURL not found")
endif (CURL_FOUND)

include(GNUInstallDirs)
include_directories("${CMAKE_SOURCE_DIR}/lib")

add_library(vault SHARED
    include/VaultClient.h
    src/auth/AliCloud.cpp
    src/auth/AppRole.cpp
    src/auth/AppRoleStrategy.cpp
    src/system/auth/Auth.cpp
    src/auth/LdapStrategy.cpp
    src/auth/WrappedSecretAppRoleStrategy.cpp
    src/domain/VaultClient.cpp
    src/domain/ConfigBuilder.cpp
    src/engines/ActiveDirectory.cpp
    src/engines/AliCloud.cpp
    src/engines/Cubbyhole.cpp
    src/engines/Database.cpp
    src/engines/KeyValue.cpp
    src/engines/Pki.cpp
    src/engines/RabbitMq.cpp
    src/engines/SSH.cpp
    src/engines/Totp.cpp
    src/engines/Transit.cpp
    src/support/Base64.cpp
    src/support/HttpClient.cpp
    src/support/HttpConsumer.cpp
    src/system/audit/Audit.cpp
    src/system/auditing/Auditing.cpp
    src/system/config/ControlGroup.cpp
    src/system/config/Cors.cpp
    src/system/config/UI.cpp
    src/system/counters/Counters.cpp
    src/system/init/Init.cpp
    src/system/Sys.cpp)

set_target_properties(vault PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    PUBLIC_HEADER "include/VaultClient.h")

target_include_directories(vault PRIVATE include)
target_include_directories(vault PRIVATE src)

if(LINK_CURL)
  target_link_libraries(vault curl)
endif(LINK_CURL)

if (ENABLE_COVERAGE)
  target_link_libraries(vault gcov)
endif ()

if (UNIX)
  install(TARGETS vault
      LIBRARY DESTINATION ${CMAKE_INSTALL_DIR}
      PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  configure_file(vault.pc.in vault.pc @ONLY)
endif (UNIX)

if (ENABLE_TEST)
  find_package(Catch2 REQUIRED)
  enable_testing()

  add_executable(libvault_test test/test.cpp)

  target_include_directories(libvault_test PRIVATE include)

  target_link_libraries(libvault_test vault)
  target_link_libraries(libvault_test curl)
  target_link_libraries(libvault_test Catch2::Catch2)

  include(CTest)
  include(Catch)

  catch_discover_tests(libvault_test)

  if (ENABLE_INTEGRATION_TEST)
    add_executable(libvault_integration_test
        test/integration/TestHelpers.h
        test/integration/AppRoleTests.cpp
        test/integration/engines/KeyValueTests.cpp
        test/integration/AuthenticationStrategies.cpp
        test/integration/engines/TransitTests.cpp
        test/integration/system/SysTests.cpp
        test/integration/system/AuthTests.cpp
        test/integration/engines/TotpTests.cpp
        test/integration/engines/CubbyholeTests.cpp
        test/integration/engines/PkiTests.cpp
        test/integration/engines/RabbitMqTests.cpp
        test/integration/engines/DatabaseTests.cpp)

    target_include_directories(libvault_integration_test PRIVATE include)

    target_link_libraries(libvault_integration_test vault)
    target_link_libraries(libvault_integration_test curl)
    target_link_libraries(libvault_integration_test Catch2::Catch2)

    catch_discover_tests(libvault_integration_test)
  endif ()
endif (ENABLE_TEST)
