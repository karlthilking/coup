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
bool coup_json::meets_required() const noexcept
{
	if (!config_contains("source")) 
		return false;
	else
		return true;
}

// Throw an error related to an issue with
// coup_config.json

[[noreturn]] void coup_json::config_error(const char *e)
{
	throw std::runtime_error(e);
}

// Initializes the only class member (json object)
// with coup_config.json that should be in the project's
// root directory
// The following errors could occur during initilization:
//      - There is no coup_config.json in the root directory
//      - The minimum required fields are not specified
//        in the config file

coup_json::coup_json(const fs::path &config_file)
{
	if (!fs::exists(config_file))
		config_error(MISSING_CONFIG);

	std::ifstream input(config_file);
	input >> config;

	if (!meets_required())
		config_error(MISSING_FIELDS);
}

// default constructor
coup_json::coup_json() = default;

// destructor
coup_json::~coup_json() = default;

// copy constructor
coup_json::coup_json(const coup_json& other)
    : config(other.config)
{}

// move constructor
coup_json::coup_json(coup_json&& other) noexcept
    : config(std::move(other.config))
{}

// copy-assignment operator
coup_json& coup_json::operator=(const coup_json& other)
{
    if (this != &other)
        config = other.config;
    return *this;
}

// move-assignment operator
coup_json& coup_json::operator=(coup_json&& other) noexcept
{
    if (this != &other)
        config = std::move(other.config);
    return *this;
}

std::string coup_json::get_cpp_version() const noexcept
{
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

std::vector<std::string> 
coup_json::get_source_directories() const noexcept
{
	return config["source"];
}

std::string coup_json::get_build_directory() const noexcept
{
	return config["build"];
}

const std::vector<std::string>&
coup_json::get_compile_flags() const noexcept
{
    std::vector<std::string> compile_flags;
	return get_entry_or("compile_flags", compile_flags);
}

std::string coup_json::dump(int tab_width) const noexcept
{
	return config.dump(tab_width);
}

bool coup_json::contains(const char *key) const noexcept
{
    return config.contains(key);
}

} // namespace coup
