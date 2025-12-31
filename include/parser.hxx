#pragma once

#include <cassert>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace coup {
inline std::string create_obj_file(std::string_view src) {
  size_t dot_pos = src.rfind('.');

  if (dot_pos == std::string_view::npos) {
    throw std::runtime_error("Not a valid source file");
  }

  std::string obj{src};
  obj = obj.substr(0, dot_pos + 1);
  obj += 'o';
  return obj;
}

inline std::vector<std::string> create_obj_files(
    const std::vector<std::string>& src_files) {
  std::vector<std::string> obj_files(src_files.size());
  for (size_t i{}; i < src_files.size(); ++i) {
    obj_files[i] = create_obj_file(src_files[i]);
  }
  return obj_files;
}

inline std::optional<std::string_view> get_stem(std::string_view file) {
  if (file.empty()) {
    return std::nullopt;
  }
  size_t dot_pos = file.rfind('.');

  if (dot_pos == 0) {
    return std::nullopt;
  }
  if (dot_pos == std::string_view::npos) {
    return file;
  }

  return file.substr(0, dot_pos);
}

inline std::optional<std::string_view> get_extension(std::string_view file) {
  if (file.empty()) {
    return std::nullopt;
  }
  size_t dot_pos = file.rfind('.');

  if (dot_pos == std::string_view::npos || dot_pos == file.length() - 1) {
    return std::nullopt;
  }

  return file.substr(dot_pos + 1);
}

inline std::optional<std::string_view> get_filename(std::string_view file) {
  if (file.empty()) {
    return std::nullopt;
  }
  size_t last_slash_pos = file.rfind('/');

  if (last_slash_pos == std::string_view::npos) {
    return file;
  }

  return file.substr(last_slash_pos + 1);
}

inline const char* sv_cstr(std::string_view sv) {
  char buffer[256];
  std::memcpy(buffer, sv.data(), sv.size());
  buffer[sv.size()] = '\0';
  const char* cstr = buffer;
  return cstr;
}
}  // namespace coup
