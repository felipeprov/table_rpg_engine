cmake_minimum_required(VERSION 4.1)
project(table_rpg_engine CXX)

set(CMAKE_CXX_STANDARD 17)
add_executable(table_rpg_engine src/main.cpp)

# configure bison/flex
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(
    bison_tables_parser # target name
    ${CMAKE_CURRENT_SOURCE_DIR}/src/grammars/roll_tables.y
    ${CMAKE_CURRENT_BINARY_DIR}/generated/roll_tables_gen.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/generated/roll_tables_gen.hpp
)

FLEX_TARGET(
    flex_tables_lexer 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/grammars/roll_tables.l
    ${CMAKE_CURRENT_BINARY_DIR}/generated/roll_tables_lexer.cpp
)

ADD_FLEX_BISON_DEPENDENCY(flex_tables_lexer bison_tables_parser)

add_library(tables_parser
    ${BISON_bison_tables_parser_OUTPUTS}
    ${FLEX_flex_tables_lexer_OUTPUTS}
    src/grammars/roll_tables_driver.cpp
)

target_include_directories(tables_parser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/grammars
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)

# main executables
add_library(trpg_lib
    src/engine/engine.cpp
)

target_include_directories(trpg_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine)
target_link_libraries(trpg_lib PUBLIC tables_parser)
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
    add_executable(${name} ${ARGN} tests/test_main.cpp)

    # Link whatever your tests need
    target_link_libraries(${name} PRIVATE trpg_lib)

    # Automatically register the test with CTest
    prov_test_framework_discover_tests( ${name} )
endfunction()

enable_testing()
include(CTest)
add_unit_test(ut_hello tests/test_hello.cpp)


