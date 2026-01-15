#pragma once

#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace fs = std::filesystem;
namespace coup
{
// return value or throw runtime error
template <typename T> inline T unwrap_or_throw(std::optional<T> opt)
{
	if (opt.has_value())
	{
		return *opt;
	}
	else
	{
		throw std::runtime_error("No optional value");
	}
}

// return optional value or specified fallback value
template <typename T> inline T unwrap_or(std::optional<T> opt, T fallback)
{
	return opt.value_or(fallback);
}

// directory discovery
std::optional<fs::path>
get_root_dir_opt(const fs::path &p = fs::current_path());
std::optional<fs::path> get_src_dir_opt(const fs::path &root);
std::optional<fs::path> get_include_dir_opt(const fs::path &root);
std::optional<fs::path> get_out_dir_opt(const fs::path &root);

// directory discovery with wrapper functions for optional handling
fs::path get_root_dir(const fs::path &p = fs::current_path());
fs::path get_src_dir(const fs::path &root);
fs::path get_include_dir(const fs::path &root);
fs::path get_out_dir(const fs::path &root);

// file path string operations
std::optional<std::string> get_stem_opt(const std::string &filepath);
std::optional<std::string> get_stem_opt(const fs::path &filepath);
std::optional<std::string> get_extension_opt(const std::string &filepath);
std::optional<std::string> get_extension_opt(const fs::path &filepath);

std::string get_stem(const std::string &filepath);
std::string get_stem(const fs::path &filepath);
std::string get_extension(const std::string &filepath);
std::string get_extension(const fs::path &filepath);
std::string get_filename(const std::string &filepath);
std::string get_filename(const fs::path &filepath);
std::string replace_extension(const fs::path &filepath, const std::string &ext);

// file type checking
bool is_src_file(const fs::path &src);
bool is_header_file(const fs::path &header);
bool is_obj_file(const fs::path &obj);

// file discovery
std::vector<fs::path> find_src_files(const fs::path &src_dir);
std::vector<fs::path> find_header_files(const fs::path &include_dir);
std::vector<fs::path> find_obj_files(const fs::path &out_dir);

std::vector<fs::path> get_src_files(const fs::path &root);
std::vector<fs::path> get_header_files(const fs::path &root);
std::vector<fs::path> get_obj_files(const fs::path &root);

// file creation
fs::path make_dep_file(const fs::path &src_file);
fs::path make_obj_file(const fs::path &src_file);

// file parsing
std::string file_to_string(const fs::path &file);
std::vector<std::string> parse_dependency_file(const fs::path &dep_file);

} // namespace coup
