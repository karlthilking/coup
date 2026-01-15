/* coup_json.cxx */
#include "../include/coup_json.hxx"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

#define REQUIRED                          \
	"Required\n\t\"cpp\" (c++ version)"   \
	"\n\t\"source\" (source directories)" \
	"\n\t\"build\" (build directory)"

#define MISSING_CONFIG              \
	"No coup_config.json found in " \
	"root directory"

#define MISSING_FIELDS             \
	"Required fields are missing " \
	"from coup_config.json\n" REQUIRED

#define CPP_PATH "/usr/bin/c++"

#define EXE "a.out"

namespace fs = std::filesystem;
namespace coup
{

// Fields requiring definition in coup_config.json
//      - cpp (c++ version: i.e. c++14, c++20, etc)
//      - source (list of directories marked as source)
//      - build (build directory)

bool coup_json::meets_required() const noexcept
{
	if (!(config.contains("cpp") && config.contains("source") &&
		  config.contains("build")))
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Throw an error related to an issue with
// coup_config.json

[[noreturn]] void coup_json::config_error(const char *e)
{
	throw std::runtime_error(e);
}

coup_json::coup_json()
	: config(nlohmann::json{})
	, compile_flags({})
{
}

// Initializes the only class member (json object)
// with coup_config.json that should be in the project's
// root directory
// The following errors could occur during initilization:
//      - There is no coup_config.json in the root directory
//      - The minimum required fields are not specified
//        in the config file

coup_json::coup_json(const fs::path &config_file)
	: compile_flags({})
{
	if (!fs::exists(config_file))
		config_error(MISSING_CONFIG);

	std::ifstream input(config_file);
	input >> config;

	if (!meets_required())
		config_error(MISSING_FIELDS);

	if (config.contains("compile_flags"))
		compile_flags = config["compile_flags"];
}

std::string coup_json::get_cpp_version() const noexcept
{
	// If field "cpp" is not defined in coup_config.json
	// an error should have already been thrown
	assert(config.contains("cpp"));

	return config["cpp"];
}

std::string coup_json::get_compiler() const noexcept
{
	return get_entry_or("compiler", std::string(CPP_PATH));
}

std::string coup_json::get_executable() const noexcept
{
	return get_entry_or("executable", std::string(EXE));
}

std::vector<std::string> coup_json::get_source_directories() const noexcept
{
	return config["source"];
}

std::string coup_json::get_build_directory() const noexcept
{
	return config["build"];
}

std::vector<std::string> coup_json::get_compile_flags() const noexcept
{
	return compile_flags;
}

std::string coup_json::dump(int tab_width) const noexcept
{
	return config.dump(tab_width);
}

} // namespace coup
