#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

#include "coup_logger.hxx"
#include "coup_project.hxx"

namespace fs = std::filesystem;
namespace coup
{
// executing system calls
bool execute_system_call(const char* command);

// composing system calls
std::string make_compile_command(const fs::path& src_file);
std::string make_link_command(const std::vector<fs::path>& obj_files);
std::string make_compile_and_link_command(
    const std::vector<fs::path>& src_files);
std::string make_run_command(const fs::path& exec_file);
std::string make_mm_command(const fs::path& src_file, const fs::path& dep_file);
std::string make_system_command(const std::string& command,
                                const fs::path& file);

// utility functions to compose and execute a system call
bool compile(const fs::path& src_file);
bool compile(const std::vector<fs::path>& src_files);
bool link(const std::vector<fs::path>& obj_files);
bool compile_and_link(const std::vector<fs::path>& src_files);
bool run(const fs::path& exec_file);
bool remove_file(const fs::path& file);
bool remove_directory(const fs::path& dir);
bool make_directory(const fs::path& dir);

// return file dependencies as a vector of file names
std::vector<std::string> get_dependencies(const fs::path& src_file);
}  // namespace coup
