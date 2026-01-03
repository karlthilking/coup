#pragma once
#include <filesystem>
#include <vector>

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

  std::vector<coup_file*> dependencies;

 public:
  // constructors
  coup_file(const fs::path& s = fs::path{}, const fs::path& h = fs::path{},
            const fs::path& o = fs::path{}, const fs::path& d = fs::path{});
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

  // setters
  void set_src(const fs::path& new_src) noexcept;
  void set_header(const fs::path& new_header) noexcept;
  void set_obj(const fs::path& new_obj) noexcept;
  void set_dep(const fs::path& new_dep) noexcept;

  // add dependencies to a coup_file object
  void add_dependency(coup_file* dep) noexcept;

  // returns true if source file requires recompilation, false otherwise
  bool requires_recompile() const noexcept;

  // returns true if dependency file needs to be udpated
  bool requires_dep_update() const noexcept;
};

}  // namespace coup
