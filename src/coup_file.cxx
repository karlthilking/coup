#include <filesystem> // path, exists, last_write_time
#include <utility> // move

#include "../include/coup_file.hxx"

namespace fs = std::filesystem;
namespace coup
{
  coup_file::coup_file(const fs::path& s, const fs::path& h, const fs::path& o)
    : src_file(s),
      header_file(h),
      obj_file(o)
      s_exists(fs::exists(src_file)),
      h_exists(fs::exists(header_file)),
      o_exists(fs::exists(obj_file))
  {}

  coup_file::coup_file(fs::path&& s, fs::path&& h, fs::path&& o)
    : src_file(std::move(s)),
      header_file(std::move(h)),
      obj_file(std::move(o))
      s_exists(fs::exists(src_file)),
      h_exists(fs::exists(header_file)),
      o_exists(fs::exists(obj_file))
  {}
  
  // check if a file type (source, header, object) exists
  bool coup_file::src_exists() const noexcept { return s_exists; }
  bool coup_file::header_exists() const noexcept { return h_exists; }
  bool coup_file::obj_exists() const noexcept { return o_exists; }
  
  // getters
  const fs::path& coup_file::get_src() const noexcept { return src_file; }
  const fs::path& coup_file::get_header() const noexcept { return header_file; }
  const fs::path& coup_file::get_obj() const noexcept { return obj_file; }

  // check if source file needs to be recompiled
  bool coup_file::requires_rebuild() const noexcept
  {
    assert(s_exists());
    if(!s_exists) { return false; }

    if(fs::last_write_time(obj_file) < fs::last_write_time(src_file) ||
       fs::last_write_time(obj_file) < fs::last_write_time(header_file))
    {
      return true;
    }
    return false;
  }
}


