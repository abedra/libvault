cmake_minimum_required(VERSION 3.9)
project(vault VERSION 0.0.1 DESCRIPTION "Vault library for C++")

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
  src/VaultClient.cpp
  src/HttpClient.cpp
  src/KeyValue.cpp)

set_target_properties(vault PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
  PUBLIC_HEADER "include/VaultClient.h")

target_include_directories(vault PRIVATE include)
target_include_directories(vault PRIVATE src)

target_link_libraries(vault PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS vault
  LIBRARY DESTINATION ${CMAKE_INSTALL_DIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(vault.pc.in vault.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/vault.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
