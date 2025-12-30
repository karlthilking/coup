#pragma once

#include <filesystem> // path

namespace fs = std::filesystem;
namespace coup
{
  class coup_file
  {
  private:
    fs::path src_file;
    fs::path header_file;
    fs::path obj_file;

    bool s_exists;
    bool h_exists;
    bool o_exists;

  public:
    coup_file(const fs::path& s, const fs::path& h, const fs::path& o);
    coup_file(fs::path&& s, fs::path&& h, fs::path&& o) noexcept;
    
    bool src_exists() const noexcept;
    bool header_exists() const noexcept;
    bool obj_exists() const noexcept;

    const fs::path& get_src() const noexcept; 
    const fs::path& get_header() const noexcept;
    const fs::path& get_obj() const noexcept;

    bool requires_rebuild() const noexcept;
  };
