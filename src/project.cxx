#include <string>
#include <vector>
#include <filesystem>
#include <utility>
#include <thread>
#include "../include/project.hxx"

namespace fs = std::filesystem;

namespace coup
{
  coup_file::coup_file(const fs::path& s, const fs::path& h, const fs::path& o)
    : src_file(s), header_file(h), obj_file(o),
      src_exists(fs::exists(s)), 
      header_exists(fs::exists(h)),
      obj_exists(fs::exists(o))
  {}

  coup_file::coup_file(fs::path&& s, fs::path&& h, fs::path&& o)
    : src_file(std::move(s)),
      header_file(std::move(h)),
      obj_file(std::move(o)),
      src_exists(fs::exists(src_file)),
      header_exists(fs::exists(header_file)),
      obj_exists(fs::exists(obj_file))
  {}

  bool coup_file::src_exists() const noexcept { return src_exists; }
  bool coup_file::header_exists() const noexcept { return header_exists; }
  bool coup_file::obj_exists() const noexcept { return obj_exists; }

  const fs::path& coup_file::get_src() const noexcept { return src_file; }
  const fs::path& coup_file::get_header() const noexcept { return header_file; }
  const fs::path& coup_file::get_obj() const noexcept { return obj_file; }

  bool coup_file::requires_rebuild() const noexcept
  {
    assert(src_exists());
    if(fs::last_write_time(obj_file) < fs::last_write_time(src_file) ||
       fs::last_write_time(obj_file) < fs::last_write_time(header_file))
    {
      return true;
    }
    return false;
  }

  coup_project::coup_project(const std::vector< coup_file >& cfiles)
    : files(cfiles)
  {}

  coup_project::coup_project(std::vector< coup_file >&& cfiles)
    : files(std::move(cfiles))
  {}

  coup_project coup_project::create_project()
  {
    std::optional< fs::path > r_opt = coup::get_root();
    if(!r_opt.has_value())
    {
      throw std::runtime_error("[ERROR] No root directory could be identified.\n");
    }
    fs::path root = r_opt.value();
    std::unordered_map< std::string, std::array< fs::path, 3 >> fgroups;

    std::thread src_handler([&](){
      std::optional< fs::path > s_opt = coup::get_src_dir(root);
      if(!s_opt.has_value()) { return; }
      std::vector< fs::path > src_files = coup::get_src_files(s_opt.value());
      
      for(const fs::path& src: src_files)
      {
        std::string src_name = src.string();
        std::string_view src_stem = coup::get_stem(src_name);
        fgroups[src_stem][0] = src;
      }
    });

    std::thread header_handler([&](){
      std::optional< fs::path > i_opt = coup::get_include_dir(root);
      if(!i_opt.has_value()) { return; }
      std::vector< fs::path > header_files = coup::get_header_files(i_opt.value());

      for(const fs::path& header: header_files)
      {
        std::string header_name = header.string();
        std::string_view header_stem = coup::get_stem(header_name);
        fgroups[header_stem][1] = header;
      }
    });

    std::thread obj_handler([&](){
        std::vector< fs::path > obj_files = coup::get_obj_files(root);
      
      for(const fs::path& obj: obj_files)
      {
        std::string obj_name = obj.string();
        std::string_view obj_stem = coup::get_stem(obj_name);
        fgroups[obj_stem][2] = obj;
      }
    });
    
    src_handler.join();
    header_handler.join();
    obj_handler.join();
    
    std::vector< coup_file > cfiles;
    for(const auto& [name, files]: fgroups)
    {
      assert(!files.empty());
      cfiles.emplace_back(files[0], files[1], files[2]);
    }
    
    return coup_project(std::move(cfiles));
  }

  std::vector< coup_file > coup_project::get_files() const noexcept { return files; }
}
