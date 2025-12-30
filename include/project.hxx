#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include <stdexcept>

namespace fs = std::filesystem;

namespace coup
{   
  class coup_file
  {
  private:
    fs::path src_file;
    fs::path header_file;
    fs::path obj_file;

    bool src_exists;
    bool header_exists;
    bool obj_exists;
  public:
    coup_file(const fs::path& s, const fs::path& h, const fs::path& o);
    coup_file(fs::path&& s, fs::path&& h, fs::path&& o);
    
    bool s_exists() const noexcept;
    bool h_exists() const noexcept;
    bool o_exists() const noexcept;

    const fs::path& get_src() const noexcept;
    const fs::path& get_header() const noexcept;
    const fs::path& get_obj() const noexcept;

    bool requires_rebuild() const noexcept;
  };

  class coup_project
  {
  private:
    std::vector< coup_file > files;
    coup_project(const std::vector< coup_file >& cfiles);
    coup_project(std::vector< coup_file >&& cfiles) noexcept;
  public:
    static coup_project create_project();
    const std::vector< coup_file >& get_files() const noexcept;
  };
}
