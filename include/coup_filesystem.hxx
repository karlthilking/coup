#pragma once

#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace fs = std::filesystem;
namespace coup {
// return value or throw runtime error
template <typename T>
inline T unwrap_or_throw(std::optional<T> opt) {
  if (opt.has_value()) {
    return opt.value();
  } else {
    throw std::runtime_error("No optional value");
  }
}

// return optional value or specified fallback value
template <typename T>
inline T unwrap_or(std::optional<T>, T fallback) {
  return opt.value_or(fallback);
}

// directory discovery
inline std::optional<fs::path> get_root_dir_opt(
    const fs::path& p = fs::current_path());
inline std::optional<fs::path> get_src_dir_opt(const fs::path& root);
inline std::optional<fs::path> get_include_dir_opt(const fs::path& root);
inline std::optional<fs::path> get_out_dir_opt(const fs::path& root);

// directory discovery with wrapper functions for optional handling
inline fs::path get_root_dir(const fs::path& p = fs::current_path());
inline fs::path get_src_dir(const fs::path& root);
inline fs::path get_include_dir(const fs::path& root);
inline fs::path get_out_dir(const fs::path& root);

// file path string operations
inline std::optional<std::string> get_stem_opt(const fs::path& filepath);
inline std::optional<std::string> get_extension_opt(const fs::path& filepath);
inline std::string get_stem(const fs::path& filepath);
inline std::string get_extension(const fs::path& filepath);
inline std::string get_filename(const fs::path& filepath);
inline std::string replace_extension(const fs::path& filepath,
                                     const std::string& ext);

// file type checking
inline bool is_src_file(const fs::path& src);
inline bool is_header_file(const fs::path& header);
inline bool is_obj_file(const fs::path& obj);

// file discovery
inline std::vector<fs::path> get_src_files(const fs::path& src_dir);
inline std::vector<fs::path> get_header_files(const fs::path& include_dir);
inline std::vector<fs::path> get_obj_files(const fs::path& out_dir);

// file creation
inline fs::path make_dep_file(const fs::path& src_file);
inline fs::path make_obj_file(const fs::path& src_file);

// file parsing
inline std::vector<std::string> parse_dependency_file(const fs::path& dep_file);

}  // namespace coup
