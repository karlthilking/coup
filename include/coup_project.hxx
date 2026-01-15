/* coup_project.hxx */
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace fs = std::filesystem;
namespace coup
{
class coup_project {
private:
    std::vector<fs::path> source_directories;
    fs::path build_directory;
    coup_json coup_config;

	coup_project(const std::vector<fs::path>& source_directories_,
                 const fs::path& build_directory_,
                 const coup_json& coup_config_);

	coup_project(std::vector<fs::path>&& source_directories_,
                 fs::path&& build_directory_, 
                 coup_json&& coup_config_) noexcept;

public:
	static coup_project make_project();
    
	std::optional<std::string> execute_build(bool verbose) noexcept;

	std::optional<std::string> execute_run() noexcept;

	std::optional<std::string> execute_clean(bool verbose) noexcept;
	
    void execute_command(const std::string &command, const std::string &option);
};

} // namespace coup
