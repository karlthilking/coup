/* coup_json.cxx */
#include "../include/coup_json.hxx"
#include "../include/coup_err.hxx"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <string>
#include <cassert>
#include <stdexcept>

namespace fs = std::filesystem;
namespace coup {
    
    // Fields requiring definition in coup_config.json
    //      - cpp (c++ version: i.e. c++14, c++20, etc)
    //      - compiler (gcc or clang)
    //      - source (list of directories marked as source)
    //      - build (build directory)
    
    bool 
    coup_json::meets_required(const nlohmann::json& config)
    const noexcept
    {
        if (!(config.contains("cpp") &&
            config.contains("compiler") &&
            config.contains("source") &&
            config.contains("build")) {
             return false;
        } else {
            return true;
        }
    }
    
    // Throw an error related to an issue with 
    // coup_config.json

    [[noreturn]] void
    coup_json::config_error(const char *e) {
        throw std::runtime_error(e);
    }
    
    // Initializes the only class member (json object)
    // with coup_config.json that should be in the project's
    // root directory
    // The following errors could occur during initilization:
    //      - There is no coup_config.json in the root directory
    //      - The minimum required fields are not specified
    //        in the config file

    coup_json::coup_json(const fs::path& config_file) {
        if (!fs::exists(config_file))
            config_error(NO_CONFIG_ERR);

        std::ifstream input(config_file);
        input >> config;

        if (!meets_required())
            config_error(MISSING_FIELDS_ERR);
    }
    
    std::string
    coup_json::get_cpp_version() const noexcept
    {
        // If field "cpp" is not defined in coup_config.json
        // an error should have already been thrown
        assert(config.contains("cpp"));

        return config["cpp"];
    }

    std::string
    coup_json::get_compiler() const noexcept
    {
        // It can also be assumed that field "compiler"
        // is included in the config, or else an error
        // would have occurred prior
        assert(config.contains("compiler"));

        std::string compiler = config["compiler"];
        if (compiler == "gcc" || compiler == "gnu" ||
            compiler == "g++")
            return "g++";
        else if (compiler == "clang" || compiler == "clang++")
            return "clang++";
        else
            config_error(INVALID_ENTRY("compiler",
                                       compiler.c_str()));
    }

    std::string
    coup_json::get_executable() const noexcept
    {
        return get_entry_or("executable",
                            std::string("a.out"));
    }

    std::vector<std::string>
    coup_json::get_source_directories() const noexcept
    {
        return config["source"];
    }
    
    std::string
    coup_json::get_build_directory() const noexcept
    {
        return config["build"];
    }

    std::vector<std::string>
    coup_json::get_compile_flags const noexcept
    {
        return compile_flags;
    }

} // namespace coup
