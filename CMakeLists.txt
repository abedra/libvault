cmake_minimum_required(VERSION 3.11)
project(libvault)

set(CMAKE_CXX_STANDARD 14)

find_package(CURL)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
else(CURL_FOUND)
    message(FATAL_ERROR "CURL not found")
endif(CURL_FOUND)

add_library(libvault SHARED src/libvault.cpp src/libvault.h)

add_executable(example src/libvault.h src/libvault.cpp test.cpp src/VaultClient.cpp src/VaultClient.h src/HttpClient.cpp src/HttpClient.h src/json.hpp)
target_link_libraries(example curl)