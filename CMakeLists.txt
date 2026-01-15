cmake_minimum_required(VERSION 3.18) 
project(coup)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(O1 OFF)
option(O2 OFF)
option(O3 OFF)

if (O1)
    add_compile_options(-O1)
elseif (O2)
    add_compile_options(-O2)
elseif (O3)
    add_compile_options(-O3)
endif()

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -fsanitize=address 
    -fsanitize=undefined
)
add_link_options(
    -fsanitize=address
    -fsanitize=undefined
)

find_program(CLANG_TIDY_PROGRAM clang-tidy)
if (CLANG_TIDY_PROGRAM)
    set(CMAKE_CXX_CLANG_TIDY 
        ${CLANG_TIDY_PROGRAM};
        -header-filter=.*include/.*;
        -warnings-as-errors=*;
    )
endif()

set(COUP_HEADERS
    include/coup_filesystem.hxx 
    include/coup_system.hxx 
    include/coup_project.hxx
    include/coup_logger.hxx 
    include/coup_json.hxx 
)

set(COUP_SOURCES
    src/main.cxx
    src/coup_filesystem.cxx
    src/coup_system.cxx
    src/coup_project.cxx
    src/coup_logger.cxx
    src/coup_json.cxx
)

add_library(
    coup_lib 
    STATIC 
    ${COUP_SOURCES} 
    ${COUP_HEADERS}
)

target_include_directories(
    coup_lib 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(coup src/main.cxx)

target_link_libraries(coup PRIVATE coup_lib)

include(FetchContent)
FetchContent_Declare(
    json 
    URL
    https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

target_link_libraries(coup PRIVATE nlohmann_json::nlohmann_json)

enable_testing() 

FetchContent_Declare(
    googletest 
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest) 

add_executable(
    coup_tests 
    tests/filesystem_test.cxx 
    tests/json_test.cxx
)

target_link_libraries(
    coup_tests
    PRIVATE 
    GTest::gtest_main
    GTest::gtest
    nlohmann_json::nlohmann_json
    coup_lib
)

include(GoogleTest)
gtest_discover_tests(coup_tests)

target_compile_options(
    coup_lib
    PRIVATE 
    -fno-modules-ts
)
