cmake_minimum_required(VERSION 3.12)
project(vault VERSION 0.45.0 DESCRIPTION "Vault library for C++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG --coverage)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

option(ENABLE_TEST "Enable tests?" ON)
option(ENABLE_INTEGRATION_TEST "Enable integration tests?" OFF)
option(ENABLE_COVERAGE "Enable gcov code coverage" OFF)
option(LINK_CURL "Link curl library for vault" OFF)
option(BUILD_SHARED_LIBS "Build vault as a shared library" ON)

include(FetchContent)
include(GNUInstallDirs)

include(cmake/curl.cmake)
include(cmake/sources.cmake)
include(cmake/testing.cmake)

include_directories("${PROJECT_SOURCE_DIR}/lib")

target_include_directories(vault PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/>)

set_target_properties(vault PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    PUBLIC_HEADER "include/VaultClient.h")

target_include_directories(vault PRIVATE src)

if(LINK_CURL)
  target_link_libraries(vault curl)
endif(LINK_CURL)

if (ENABLE_COVERAGE)
  target_link_libraries(vault gcov)
endif ()

if (UNIX)
  install(TARGETS vault
      EXPORT libvaultTargets
      LIBRARY DESTINATION ${CMAKE_INSTALL_DIR}
      PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libvault)

  install(EXPORT libvaultTargets
    NAMESPACE libvault::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libvault)

  include(CMakePackageConfigHelpers)

  configure_package_config_file(
          ${CMAKE_CURRENT_SOURCE_DIR}/VaultConfig.cmake.in
          ${CMAKE_CURRENT_BINARY_DIR}/libvaultConfig.cmake
          INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libvault
          PATH_VARS
          CMAKE_INSTALL_LIBDIR)

  write_basic_package_version_file(
          ${CMAKE_CURRENT_BINARY_DIR}/libvaultConfigVersion.cmake
          COMPATIBILITY SameMajorVersion)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libvaultConfig.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/libvaultConfigVersion.cmake"
          DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libvault")

  configure_file(vault.pc.in vault.pc @ONLY)
  install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc"
          DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
endif (UNIX)
