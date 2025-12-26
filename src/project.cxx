#include <string>
#include <vector>
#include <filesystem>
#include <utility>
#include "../include/project.hxx"

namespace fs = std::filesystem;

namespace coup
{
  coupfile::coupfile(const fs::path& s, const fs::path& h, const fs::path& o)
    : src_file(s), header_file(h), obj_file(o),
      src_exists(fs::exists(s)), 
      header_exists(fs::exists(h)),
      obj_exists(fs::exists(o))
  {}

  coupfile::coupfile(fs::path&& s, fs::path&& h, fs::path&& o)
    : src_file(std::move(s)),
      header_file(std::move(h)),
      obj_file(std::move(o)),
      src_exists(fs::exists(src_file)),
      header_exists(fs::exists(header_file)),
      obj_exists(fs::exists(obj_file))
  {}

  bool coupfile::src_exists() const noexcept { return src_exists; }
  bool coupfile::header_exists() const noexcept { return header_exists; }
  bool coupfile::obj_exists() const noexcept { return obj_exists; }

  const fs::path& coupfile::get_src() const noexcept { return src_file; }
  const fs::path& coupfile::get_header() const noexcept { return header_file; }
  const fs::path& coupfile::get_obj() const noexcept { return obj_file; }

  bool 
