cmake_minimum_required(VERSION 3.9)
project(vault VERSION 0.0.5 DESCRIPTION "Vault library for C++")

set(CMAKE_CXX_STANDARD 14)


option(ENABLE_TEST "Enable tests?" OFF)




find_package(CURL)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
else(CURL_FOUND)
    message(WARNING "CURL not found")
endif(CURL_FOUND)


if(MSCV)
endif(MSCV)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc /std:c++latest")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")



message(${CURL_LIBRARIES})
message(${CURL_INCLUDE_DIRS})

include(GNUInstallDirs)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
include_directories("${CMAKE_SOURCE_DIR}/external/nlohmann_json/single_include" "${CMAKE_SOURCE_DIR}/external/optional" )

add_library(vault SHARED
  include/VaultClient.h
  src/VaultHttpConsumer.cpp
  src/VaultClient.cpp
  src/HttpClient.cpp
  src/KeyValue.cpp
  src/Transit.cpp
  src/AppRole.cpp)

set_target_properties(vault PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
  PUBLIC_HEADER "include/VaultClient.h")

target_include_directories(vault PRIVATE include )
target_include_directories(vault PRIVATE src)

target_link_libraries(vault ${CURL_LIBRARIES})

if (UNIX)
  set(CMAKE_INSTALL_DIR )
  install(TARGETS vault
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


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

