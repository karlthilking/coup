#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace coup {

std::optional<std::string_view> inline extract_header(std::string_view line) {
  if (line.empty()) {
    return std::nullopt;
  }
  if (line.find("#include") == std::string_view::npos) {
    return std::nullopt;
  }

  size_t l = line.find('"') + 1;
  if (l == std::string::npos) {
    return std::nullopt;
  }
  size_t r = line.rfind('"');
  if (r == std::string::npos) {
    return std::nullopt;
  }

  return line.substr(l, r - l);
}

}  // namespace coup
