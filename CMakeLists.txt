cmake_minimum_required(VERSION 3.9)
project(vault VERSION 0.0.6 DESCRIPTION "Vault library for C++")

set(CMAKE_CXX_STANDARD 14)

find_package(CURL)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
else(CURL_FOUND)
    message(FATAL_ERROR "CURL not found")
endif(CURL_FOUND)

include(GNUInstallDirs)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)

add_library(vault SHARED
  include/VaultClient.h
  src/VaultHttpConsumer.cpp
  src/VaultClient.cpp
  src/HttpClient.cpp
  src/KeyValue.cpp
  src/Transit.cpp
        src/AppRoleStrategy.cpp
  src/Unwrap.cpp
  src/WrappedSecretAppRole.cpp
)

set_target_properties(vault PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
  PUBLIC_HEADER "include/VaultClient.h"
)

target_include_directories(vault PRIVATE include)
target_include_directories(vault PRIVATE src)

target_link_libraries(vault PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS vault
  LIBRARY DESTINATION ${CMAKE_INSTALL_DIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_file(vault.pc.in vault.pc @ONLY)

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
