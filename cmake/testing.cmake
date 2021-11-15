if (ENABLE_TEST)
    FetchContent_Declare(
            Catch2
            GIT_SHALLOW    TRUE
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v2.13.6)
    FetchContent_MakeAvailable(Catch2)

    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
    enable_testing()

    add_executable(libvault_test test/test.cpp)

    target_include_directories(libvault_test PRIVATE include)

    target_link_libraries(libvault_test vault curl Catch2::Catch2)

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
                test/integration/engines/DatabaseTests.cpp
                test/integration/TokensTests.cpp)

        target_include_directories(libvault_integration_test PRIVATE include)

        target_link_libraries(libvault_test vault curl Catch2::Catch2)

        catch_discover_tests(libvault_integration_test)
    endif ()
endif (ENABLE_TEST)
