#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cassert>

#define REQUIRED "Required fields coup_config.json\n\t\"compiler\" (gcc or clang)\n\t\"cpp\" (i.e. c++20)
                 \n\t\"build_directory\" (relative to root path)\n\t\"source_files\" or \"source_directories\"
                 (relative to root path)\n"

#define MISSING_CONFIG "Please create a coup_config.json with the necessary fields\n" REQUIRED \
                       "\n--help for more\n"
#define MISSING_FIELDS "coup_config.json is missing required fields\n" REQUIRED "\n--help for more\n"

#define INVALID_ENTRY(k) "Invalid entry in coup_config.json for: "#k
                    

namespace fs = std::filesystem;
namespace coup {

/*  requires that user config contains
 *  at least compiler and cpp version info
 */
coup_json::meets_required() const noexcept
{
    if (config.contains("cpp") &&
        config.contains("compiler"))
        return true;
    } else {
        return false;
    }
}

[[noreturn]] void
coup_json::config_error(const char *e)
{
    throw std::runtime_error(e);
}

coup_json::coup_json(const fs::path& config_file)
{
    if (!fs::exists(config_file))
        config_error(MISSING_CONFIG);

    std::ifstream input(config_file);
    input >> config;
    
    if (!meets_required()) 
        config_error(MISSING_FIELDS);
}

std::string
coup_json::get_compiler() const noexcept {
    std::string compiler = config["compiler"];
    if (compiler == "gcc" || compiler == "gnu" ||
        compiler == "g++")
        return "g++";
    else if (compiler == "clang" || compiler == "clang++")
        return "clang++";
    else {
        // should never reach here
        assert(false);
    }
}

std::string
coup_json::get_cpp_version() const noexcept {
    return config["cpp"];
}

std::string
coup_json::get_executable() const noexcept {
    return get_entry_or(std::string("executable"),
                        std::string(""));
}

std::vector<std::string> 
coup_json::get_source_directories() const noexcept
{
    return get_entry_or(std::string("source_directories"),
                                    {});
}

std::string get_build_directory() const noexcept
{
    return get_entry_or(std::string("build_directory",
                        std::string(""));
}

std::vector<std::string>
get_source_files() const noexcept
{
    return get_entry_or(std::string("source_files", {});
}

std::vector<std::string>
get_compile_flags() const noexcept
{
    return get_entry_or(std::string("compile_flags", {});
}

} // namespace coup

