#include <filesystem>
#include <vector>
#include "../include/coup_file.hxx"

namespace fs = std::filesystem;
namespace coup {
// copies std::filesystem::path's to coup_file member variables
coup_file::coup_file(const fs::path& s, const fs::path& h, const fs::path& o,
                     const fs::path& d)
    : src_file(s),
      header_file(h),
      obj_file(o),
      dep_file(d),
      src_exists(fs::exists(src_file)),
      header_exists(fs::exists(header_file)),
      obj_exists(fs::exists(obj_file)),
      dep_exists(fs::exists(dep_file)) {}

// move std::filesystem::path's to coup_file member variables
coup_file::coup_file(fs::path&& s, fs::path&& h, fs::path&& o,
                     fs::path&& d) noexcept
    : src_file(std::move(s)),
      header_file(std::move(h)),
      obj_file(std::move(o)),
      dep_file(std::move(d)),
      src_exists(fs::exists(src_file)),
      header_exists(fs::exists(header_file)),
      obj_exists(fs::exists(obj_file)),
      dep_exists(fs::exists(dep_file)) {}

// returns true if _ file exists, false otherwise
bool coup_file::has_src() const noexcept { return src_exists; }
bool coup_file::has_header() const noexcept { return header_exists; }
bool coup_file::has_obj() const noexcept { return obj_exists; }
bool coup_file::has_dep() const noexcept { return dep_exists; }

// returns a const reference to a std::filesystem::path member
// of a coup_file reference
const fs::path& coup_file::get_src() const noexcept { return src_file; }
const fs::path& coup_file::get_header() const noexcept { return header_file; }
const fs::path& coup_file::get_obj() const noexcept { return obj_file; }
const fs::path& coup_file::get_dep() const noexcept { return dep_file; }

// set a file to a new path variable
void coup_file::set_src(const fs::path& new_src) noexcept {
  if (!src_exists) src_exists = true;
  src_file = new_src;
}
void coup_file::set_header(const fs::path& new_header) noexcept {
  if (!header_exists) header_exists = true;
  header_file = new_header;
}
void coup_file::set_obj(const fs::path& new_obj) noexcept {
  if (!obj_exists) obj_exists = true;
  obj_file = new_obj;
}
void coup_file::set_dep(const fs::path& new_dep) noexcept {
  if (!dep_exists) dep_exists = true;
  dep_file = new_dep;
}

// add a dependency to the coup_file object's list
void coup_file::add_dependency(coup_file* dep) noexcept {
  dependencies.push_back(dep);
}

/*
 * Returns true if the source file needs to be compiled/recompiled, false
 * otherwise
 *
 * The file needs to be compiled if:
 *  - no object file exists
 *  - header or source has been updated more recently than object file
 *  - header or source of any dependency has been updated after the object file
 */
bool coup_file::requires_recompile() const noexcept {
  if (!src_exists) {
    return false;
  } else if (!obj_exists) {
    return true;
  } else {
    fs::file_time_type obj_update = fs::last_write_time(obj_file);
    if (obj_update < fs::last_write_time(src_file)) {
      return true;
    } else if (header_exists && obj_update < fs::last_write_time(header_file)) {
      return true;
    } else {
      for (coup_file* dep : dependencies) {
        if (dep->has_src() &&
            obj_update < fs::last_write_time(dep->get_src())) {
          return true;
        } else if (dep->has_header() &&
                   obj_update < fs::last_write_time(dep->get_header())) {
          return true;
        }
      }
      return false;
    }
  }
}

// Returns true if dependency file needs to be set or updated
bool coup_file::requires_dep_update() const noexcept {
  if (!src_exists) {
    return false;
  } else if (!dep_exists) {
    return true;
  } else {
    fs::file_time_type dep_update = fs::last_write_time(dep_file);
    if (dep_update < fs::last_write_time(src_file) ||
        (header_exists && dep_update < fs::last_write_time(header_file))) {
      return true;
    }
    return false;
  }
}
}  // namespace coup
