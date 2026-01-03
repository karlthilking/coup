cmake_minimum_required(VERSION 3.18)
project(coup VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_ASAN OFF)
option(ENABLE_UBSAN OFF)
option(ENABLE_TSAN OFF)
option(ENABLE_WARNINGS ON)
option(WARNINGS_AS_ERRORS OFF)
option(ENABLE_CLANG_TIDY OFF)
option(ENABLE_TESTING ON)

if(ENABLE_WARNINGS)
	add_compile_options(
		-Wall,
		-Wextra,
		-Wpedantic,
	)
	if(WARNINGS_AS_ERRORS)
		add_compile_options(-Werror)
	endif()
endif()

if(ENABLE_ASAN)
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

if(ENABLE_UBSAN)
	add_compile_options(-fsanitize=undefined)
	add_link_options(-fsanitize=undefined)
endif()

if(ENABLE_TSAN)
	add_compile_options(-fsanitize=thread)
	add_link_options(-fsanitize=thread)
endif()

if(ENABLE_CLANG_TIDY)
	find_program(CLANG_TIDY_PROGRAM clang-tidy)
	if(CLANG_TIDY_PROGRAM)
		set(CMAKE_CXX_CLANG_TIDY
			${CLANG_TIDY_PROGRAM};
			-header-filter=.*include/.*;
			-warnings-as-errors=*;
		)
	endif()
endif()

set(COUP_HEADERS
	include/coup_filesystem.hxx
  include/coup_system.hxx
  include/coup_file.hxx
  include/coup_project.hxx
)

set(COUP_SOURCES
  src/main.cxx
  src/coup_filesystem.cxx
  src/coup_system.cxx
  src/coup_file.cxx
  src/coup_project.cxx
)

add_library(coup_lib STATIC ${COUP_SOURCES} ${COUP_HEADERS})

target_include_directories(coup_lib PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(coup
  src/main.cxx
)

target_link_libraries(coup PRIVATE coup_lib)

if(ENABLE_TESTING)
  add_compile_definitions(ENABLE_TESTING)
  enable_testing()
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
  )

  FetchContent_MakeAvailable(googletest)
  add_executable(coup_tests
    tests/filesystem_test.cxx
  )
  target_link_libraries(coup_tests PRIVATE
    coup_lib
    GTest::gtest_main
  )
  include(GoogleTest)
  gtest_discover_tests(coup_tests)
endif()
