cmake_minimum_required(VERSION 3.11)
project(vault VERSION 0.0.1 DESCRIPTION "Vault library for C++")

set(CMAKE_CXX_STANDARD 14)

find_package(CURL)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
else(CURL_FOUND)
    message(FATAL_ERROR "CURL not found")
endif(CURL_FOUND)

add_library(vault SHARED src/VaultClient.cpp src/VaultClient.h src/HttpClient.cpp src/HttpClient.h src/json.hpp)
set_target_properties(vault PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(vault PROPERTIES SOVERSION 1)
set_target_properties(vault PROPERTIES PUBLIC_HEADER src/VaultClient.h)
target_link_libraries(example curl)
