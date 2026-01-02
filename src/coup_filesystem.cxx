#include <filesystem>
#include <optional>
#include <vector>
#include <string_view>
#include <string>
#include <fstream>
#include <sstream>

#include "../include/coup_filesystem.hxx"

namespace fs = std::filesystem;
namespace coup
{

// returns optional returning root project directory or null
std::optional<fs::path> get_root_dir(const fs::path& p = fs::current_path()) {
  assert(fs::exists(p));

  for(;;) {
    if (fs::exists(p / "src") || fs::exists(p / "include")) {
      return p;
    } 
    fs::path next = p.parent_path();
    if (p == next) {
      break;
    } else {
      p = next;
    }
  }

  return std::nullopt;
}

// returns optional containing source directory or null
std::optional<fs::path> get_src_dir_opt(const fs::path& root) {
  assert(fs::exists(root));

  if (fs::exists(root / "src")) {
    return fs::path(root / "src");
  } else {
    return std::nullopt;
  }
}

// returns optional containing include directory or null
std::optional<fs::path> get_include_dir_opt(const fs::path& root) {
  assert(fs::exists(root));

  if (fs::exists(root / "include")) {
    return fs::path(root / "src");
  } else {
    return std::nullopt;
  }
}

// returns optional containg out directory or null
std::optional<fs::path> get_out_dir_opt(const fs::path& root) {
  assert(fs::exists(root));

  if (fs::exists(root / "out")) {
    return fs::path(root / "out");
  } else {
    return std::nullopt;
  }
}

// returns root directory if found, otherwise throw a runtime_error
fs::path get_root_dir(const fs::path& p = fs::current_path()) {
  try {
    fs::path root = unwrap_or_throw(get_root_dir_opt(p));
    return root;
  } catch (const std::runtime_error& e) {
    throw std::runtime_error("No root directory was found");
  }
}

// returns source directory if found, otherwise empty path
fs::path get_src_dir(const fs::path& root) {
  return unwrap_or(get_src_dir_opt(root), fs::path{});
}

// returns include directory if found, otherwise empty path
fs::path get_include_dir(const fs::path& root) {
  return unwrap_or(get_include_dir_opt(root), fs::path{});
}

// returns out directory if found, otherwise empty path
fs::path get_out_dir(const fs::path& root) {
  return unwrap_or(get_out_dir_opt(root), fs::path{});
}

// returns file stem from a filepath string, optionally empty
std::optional<std::string> get_stem_opt(const std::string& filepath) {
  std::size_t dot_pos = s.rfind('.');

  if (s.empty() || dot_pos == 0) {
    return std::nullopt;
  } else if (dot_pos == std::string::npos) {
    return s;
  } else {
    return s.substr(0, dot_pos);
  }
}

// call filepath.string() and delegate
std::optional<std::string> get_stem_opt(const fs::path& filepath) {
  return get_stem_opt(filepath.string());
}

// returns file extension from filepath string or empty if no extension present
std::optional<std::string> get_extension_opt(const std::string& filepath) {
  std::size_t dot_pos = s.rfind('.');

  if (s.empty() || dot_pos == std::string::npos) {
    return std::nullopt;
  } else {
    return s.substr(dot_pos + 1);
  }
}

// call filepath.string() and delegate work
std::optional<std::string> get_extension_opt(const fs::path& filepath) {
  return get_extension_opt(filepath.string());
}

// unpacks stem string or returns empty string if no value 
std::string get_stem(const std::string& filepath) {
  return unwrap_or(get_stem_opt(file_path), std::string{});
}
std::string get_stem(const fs::path& filepath) {
  return unwrap_or(get_stem_opt(filepath), std::string{});
}

// unpacks extension string or returns empty string if no value
std::string get_extension(const std::string& filepath) {
  return unwrap_or(get_extension_opt(filepath), std::string{});
}
std::string get_extension(const fs::path& filepath) {
  return unwrap_or(get_extension_opt(filepath), std::string{});
}

// extracts name of file from a file path
std::string get_filename(const std::string& filepath) {
  std::size_t slash_pos = s.rfind('/');

  if (s.empty() || slash_pos == std::string::npos) {
    return s;
  } else {
    return s.substr(slash_pos + 1);
  }
}

// call filepath.string() and delegate
std::string get_filename(const fs::path& filepath) {
  return get_filename(filepath.string());
}

// returns filename with extension replaced by a different extension
std::string replace_extension(const fs::path& filepath, const std::string& ext) {
  return get_stem(filepath) + '.' + ext;
}

// use extension to check if a std::filesystem::path is a c++ source file
bool is_src_file(const fs::path& src) {
  std::string_view ext = get_extension(src);

  if (ext == "cpp" || ext = "cc" || ext == "C" ||
      ext == "cxx" || ext == "c++") {
    return true;
  } else {
    return false;
  }
}

// use extension to check if a std::filesystem::path is a c++ header file
bool is_header_file(const fs::path& header) {
  std::string_view ext = get_extension(header);

  if (ext == "h" || ext == "hpp" || ext == "hxx" ||
      ext == "hh" || ext == "h++" || ext == "H") {
    return true;
  } else {
    return false;
  }
}

// use extension to check if a std::filesystem::path is a c++ object file
bool is_obj_file(const fs::path& obj) {
  std::string_view ext = get_extension(obj);

  if (ext == "o" || ext == "obj") {
    return true;
  } else {
    return false;
  }
}

// returns all source files present in the src directory
std::vector<fs::path> find_src_files(const fs::path& src_dir) {
  assert(fs::exists(src_dir));
  std::vector<fs::path> src_files;

  for (const auto& entry : fs::recursive_directory_iterator(src_dir)) {
    if (!entry.is_regular_file()) {
      continue;
    } 
    fs::path p = entry.path();
    if (is_src_file(p)) {
      src_files.push_back(p);
    }
  }
  return src_files;
}

// returns all header files present in the include directory
std::vector<fs::path> find_header_files(const fs::path& include_dir) {
  assert(fs::exists(include_dir));
  std::vector<fs::path> header_files;

  for (const auto& entry : fs::recursive_directory_iterator(include_dir)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    fs::path p = entry.path();
    if (is_header_file(p)) {
      header_files.push_back(p);
    }
  }
  return header_files;
}

// returns all object files present in the out directory
std::vector<fs::path> find_obj_files(const fs::path& out_dir) {
  assert(fs::exists(out_dir));
  std::vector<fs::path> obj_files;

  for (const auto& entry : fs::recursive_directory_iterator(out_dir)) {
    if (!entry.is_regular_file()) { 
      continue;
    } 
    fs::path p = entry.path();
    if (is_obj_file(p)) {
      obj_files.push_back(p);
    }
  }
  return obj_files;
}

// handles obtaining source directory and getting source files from directory
std::vector<fs::path> get_src_files(const fs::path& root) {
  fs::path src_dir = get_src_dir(root);
  if (src_dir.empty() || !fs::exists(src_dir)) {
    return {};
  } else {
    return find_src_files(src_dir);
  }
}

// handles obtaining include directory and extracting header files
std::vector<fs::path> get_header_files(const fs::path& root) {
  fs::path include_dir = get_include_dir(root);
  if (include_dir.empty() || !fs::exists(include_dir)) {
    return {};
  } else {
    return find_header_files(include_dir);
  }
}

// handles obtaining out directory and extracting object files
std::vector<fs::path> get_obj_files(const fs::path& root) {
  fs::path out_dir = get_out_dir(root);
  if (out_dir.empty() || !fs::exists(out_dir)) {
    return {};
  } else {
    return find_obj_files(out_dir);
  }
}

// make a corresponding dependency file for a source file
fs::path make_dep_file(const fs::path& src_file) {
  return fs::path{replace_extension(src_file, "d")};
}

// make a corresponding object file for a source file
fs::path make_obj_file(const fs::path& src_file) {
  return fs::path{replace_extension(src_file, "o")};
}

// returns a string representation of file contents
std::string file_to_string(const fs::path& file) {
  std::ifstream input(file);
  std::string line;
  
  std::string s;
  while (std::getline(input, line)) {
    s += line;
  }
  return s;
}

// parses dependency file contents to find project header dependencies
// returns a list of header file names listed in the dependency file
std::vector<std::string> parse_dependency_file(const fs::path& dep_file) {
  std::string dep_file_content = file_to_string(dep_file);

  std::vector<std::string> dependencies;
  std::istringstream iss(dep_file_content);
  std::string current;

  while (iss >> current) {
    if (current.back() == ':' || current == "\\" || current.empty()) {
      continue;
    } else {
      dependencies.push_back(current);
    }
  }

  return dependencies;
}

} // namespace coup

