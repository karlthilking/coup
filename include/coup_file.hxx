#pragma once

#include <filesystem>
#include <vector>
#include <memory>

namespace fs = std::filesystem;
namespace coup {

class coup_file {
private:
  fs::path src_file;
  fs::path header_file;
  fs::path obj_file;
  fs::path dep_file;

  bool src_exists;
  bool header_exists;
  bool obj_exists;
  bool dep_exists;

  std::vector<std::shared_ptr<coup_file>> dependencies;
  
public:
  // constructors
  coup_file(const fs::path& s, const fs::path& h, const fs::path& o, const fs::path& d);
  coup_file(fs::path&& s, fs::path&& h, fs::path&& o, fs::path&& d) noexcept;
  
  // functions to check if a coup_file contains a specific file type
  bool has_src() const noexcept;
  bool has_header() const noexcept;
  bool has_obj() const noexcept;
  bool has_dep() const noexcept;
  
  // getters
  const fs::path& get_src() const noexcept;
  const fs::path& get_header() const noexcept;
  const fs::path& get_obj() const noexcept;
  const fs::path& get_dep() const noexcept;
  const std::vector<std::shared_ptr<coup_file>> get_dependencies() const noexcept;
  
  // add dependencies to a coup_file object
  void add_dependency(std::shared_ptr<coup_file> dep) noexcept;
  void add_dependencies(std::vector<std::shared_ptr<coup_file>> deps) noexcept;
  
  // returns true if a coup_file's source file requires recompilation, false otherwise
  bool requires_recompile() const noexcept;
};

} // namespace coup
