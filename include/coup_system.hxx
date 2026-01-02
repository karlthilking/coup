#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;
namespace coup {
// executing system calls
inline bool execute_system_call(const char* command);

// composing system calls
inline std::string make_compile_command(const fs::path& src_file);
inline std::string make_link_command(const std::vector<fs::path>& obj_files);
inline std::string make_compile_and_link_command(
    const std::vector<fs::path>& src_files);
inline std::string make_run_command(const fs::path& exec_file);
inline std::string make_mm_command(const fs::path& src_file,
                                   const fs::path& dep_file);
inline std::string make_system_command(const std::string& command,
                                       const fs::path& file);

// utility functions to compose and execute a system call
inline bool compile(const fs::path& src_file);
inline bool compile(const std::vector<fs::path>& src_files);
inline bool link(const std::vector<fs::path>& obj_files);
inline bool compile_and_link(const std::vector<fs::path>& src_files);
inline bool run(const fs::path& exec_file);
inline bool remove_file(const fs::path& file);
inline bool remove_directory(const fs::path& dir);
inline bool make_directory(const fs::path& dir);

// return file dependencies as a vector of file names
inline std::vector<std::string> get_dependencies(const fs::path& src_file);

}  // namespace coup
