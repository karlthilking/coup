#include <filesystem> // path
#include <vector>
#include <thread>
#include <optional> // file_tracker functions return optionals
#include <stdexcept> // runtime_error
#include <cassert> // assert
#include <unordered_map>
#include "../include/coup_project.hxx"
#include "../include/coup_file.hxx" // coup_file
#include "../include/file_tracker.hxx" // get_root, get_src_dir, get_include_dir,
                                       // get_src_files, get_header_files, get_obj_files

namespace fs = std::filesystem;
namespace coup
{
  // PRIVATE CONSTRUCTORS
  coup_project::coup_project(const std::vector< coup_file >& coup_files)
    : files(cfiles)
  {}

  coup_project::coup_project(std::vector< coup_file >&& coup_files) noexcept
    : files(std::move(cfiles))
  {}
  
  // COUP PROJECT STATIC FACTORY 
  coup_project coup_project::create_project()
  {
    std::optional< fs::path > r_opt = coup::get_root();
    if(!r_opt.has_value())
    {
      throw std::runtime_error("[ERROR] No project directory was identified\n");
    }
    fs::path root = r_opt.value();

    std::vector< fs::path > src_files;
    std::vector< fs::path > header_files;
    std::vector< fs::path > obj_files;

    // source file finder thread
    std::thread src_finder([&]
    {
      std::optional< fs::path > s_opt = coup::get_src_dir(root);
      if(!s_opt.has_value()) { return; }
      src_files = coup::get_src_files(s_opt.value());
    }

    // header file finder thread
    std::thread header_finder([&]
    {
      std::optional< fs::path > i_opt = coup::get_include_dir(root);
      if(!i_opt.has_value()) { return; }
      header_files = coup::get_header_files(i_opt.value());
    }

    // object file finder thread
    std::thread obj_finder([&]
    {
      obj_files = coup::get_obj_files(root);
    }
    
    src_finder.join();
    header_finder.join();
    obj_finder.join();
    
    // filename -> { source, header, object }
    std::unordered_map< std::string, std::array< fs::path, 3>> coup_file_args;
    std::vector< coup_file > coup_files;
    
    for(const fs::path& src: src_files)
    {
      std::string src_name = src.string();
      std::string_view filename = coup::get_stem(src_name);
      coup_file_args[filename][0] = src;
    }

    for(const fs::path& header: header_files)
    {
      std::string header_name = header.string();
      std::string_view filename = coup::get_stem(header_name);
      coup_file_args[filename][1] = header;
    }

    for(const fs::path& obj: obj_files)
    {
      std::string obj_name = obj.string();
      std::string_view filename = coup::get_stem(obj_name);
      coup_file_args[filename][2] = obj;
    }
    
    assert(!coup_file_args.empty());
    for(const auto& [filename, file_types]: coup_file_args)
    {
      coup_files.emplace_back(file_type[0], file_types[1], file_types[2]);
    }
    
    assert(!coup_files.empty());
    return coup_project(std::move(coup_files));
  }
  
  // return all project files to function caller
  [[nodiscard]] const std::vector< coup_file >&
  coup_project::get_files() const noexcept { return files; }
}
