cmake_minimum_required(VERSION 3.9)
project(vault VERSION 0.2.2 DESCRIPTION "Vault library for C++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_TEST "Enable tests?" ON)

find_package(CURL)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
else(CURL_FOUND)
    message(FATAL_ERROR "CURL not found")
endif(CURL_FOUND)

include(GNUInstallDirs)
include_directories("${CMAKE_SOURCE_DIR}/lib")

add_library(vault SHARED
    include/VaultClient.h
    src/VaultHttpConsumer.cpp
    src/VaultClient.cpp
    src/HttpClient.cpp
    src/KeyValue.cpp
    src/Transit.cpp
    src/AppRoleStrategy.cpp
    src/Unwrap.cpp
    src/WrappedSecretAppRoleStrategy.cpp
    src/Base64.cpp
    src/LdapStrategy.cpp
    src/VaultConfigBuilder.cpp)

set_target_properties(vault PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
  PUBLIC_HEADER "include/VaultClient.h"
)

target_include_directories(vault PRIVATE include)
target_include_directories(vault PRIVATE src)
target_link_libraries(vault curl)

if(UNIX)
install(TARGETS vault
  LIBRARY DESTINATION ${CMAKE_INSTALL_DIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_file(vault.pc.in vault.pc @ONLY)
endif(UNIX)

if(ENABLE_TEST)
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
endif(ENABLE_TEST)
