cmake_minimum_required(VERSION 4.1)
project(table_rpg_engine C)

set(CMAKE_C_STANDARD 23)
add_executable(table_rpg_engine src/main.c)

# configure bison/flex
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# main executables
add_library(trpg_lib
    src/core/engine.c
    src/core/pallocator.c
    src/core/parena.c
    src/core/parray.c
    src/core/pregex.c
)

target_include_directories(trpg_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/core)
target_link_libraries(table_rpg_engine PRIVATE trpg_lib)


# Enable testing

function(prov_test_framework_discover_tests target)
    # Figure out where the exe will live
    get_target_property(loc ${target} RUNTIME_OUTPUT_DIRECTORY)
    if(NOT loc)
        set(loc "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    if(WIN32)
        set(test_binary "${loc}/${target}.exe")
    else()
        set(test_binary "${loc}/${target}")
    endif()

    # First configure: exe does not exist yet
    if(NOT EXISTS "${test_binary}")
        message(WARNING
            "Test binary '${test_binary}' does not exist yet; "
            "build the target '${target}' once, then re-configure to discover tests.")
        return()
    endif()

    # Now we know the exe exists, we can safely run it
    execute_process(
        COMMAND "${test_binary}" --list
        OUTPUT_VARIABLE test_list_raw
        RESULT_VARIABLE result
    )

    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Failed to list tests for ${target}")
    endif()

    # Split output into lines
    string(REPLACE "\r\n" "\n" test_list_raw "${test_list_raw}")
    string(REPLACE "\n" ";" test_list "${test_list_raw}")
    
    foreach(test_name IN LISTS test_list)
        if(test_name STREQUAL "")
            continue()
        endif()

        add_test(
            NAME "${target}.${test_name}"
            COMMAND "${test_binary}" --run "${test_name}"
        )
    endforeach()
endfunction()

function(add_unit_test name)
    # All the sources for this test executable
    add_executable(${name} ${ARGN} tests/test_main.c tests/test_framework.c)

    # Link whatever your tests need
    target_link_libraries(${name} PRIVATE trpg_lib)

    # Automatically register the test with CTest
    prov_test_framework_discover_tests( ${name} )
endfunction()

enable_testing()
include(CTest)
add_unit_test(ut_hello tests/test_hello.c)
add_unit_test(ut_regex tests/test_regex.c)


