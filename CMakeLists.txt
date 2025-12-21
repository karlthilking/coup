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
	source/commands.hxx
	source/file_tracker.hxx
	source/project.hxx
	source/utility.hxx
	source/config.hxx
	source/regex.hxx
)
	
add_executable(coup source/main.cxx ${COUP_HEADERS})

target_include_directories(coup PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/source
)

