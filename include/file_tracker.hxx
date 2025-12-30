#pragma once

#include <filesystem> // path, last_write_time, current_path, parent_path, exists
#include <fstream> // ifstream
#include <vector>
#include <string_view>
#include <optional>
#include <cassert> // assert
#include <stdexcept> // runtime_error
#include <unordered_map>
#include <algorithm> // find

#include "regex.hxx" // extract_header
#include "parser.hxx" // get_stem, get_extension

namespace fs = std::filesystem; 

namespace coup 
{
  inline std::optional< fs::path > get_root(const fs::path& p = fs::current_path())
  {
    fs::path cur = fs::absolute(p);
    assert(fs::exists(p));

    for(;;)
    {
      if(fs::exists(cur / "src") || fs::exists(cur / "include"))
      {
        return cur;
      }
      fs::path next = cur.parent_path();
      if(cur == next) { break; }
      cur = next;
    }
    return std::nullopt;
  }

  inline std::optional< fs::path > get_src_dir(const fs::path& root)
  {
    if(fs::exists(root / "src"))
    {
      return fs::path(root / "src");
    }
    return std::nullopt;
  }

  inline std::optional< fs::path > get_include_dir(const fs::path& root)
  {
    if(fs::exists(root / "include"))
    {
      return fs::path(root / "include");
    }
    return std::nullopt;
  }

  [[nodiscard]] inline bool is_src_file(const fs::path& file)
  {
    std::string filepath = file.string();
    std::optional< std::string_view > ext_opt = get_extension(filepath);

    if(!ext_opt.has_value()) { return false; }
    
    std::string_view file_ext = ext_opt.value();
	  if(file_ext == "cpp" ||
			 file_ext == "cc" ||
       file_ext == "cxx" ||
			 file_ext == "C" ||
			 file_ext == "c++")
		{
			return true;
		}
		return false;
	}
  
  [[nodiscard]] inline bool is_header_file(const fs::path& file)
  {
    std::string filepath = file.string();
    std::optional< std::string_view > ext_opt = get_extension(filepath);

    if(!ext_opt.has_value()) { return false; }
    
    std::string_view file_ext = ext_opt.value();
		if(file_ext == "h" ||
			 file_ext == "hh" ||
			 file_ext == "hpp" ||
			 file_ext == "hxx" ||
			 file_ext == "h++" ||
			 file_ext == "H")
		{
			return true;
		}
    return false;
	}

  [[nodiscard]]
  inline bool is_obj_file(const fs::path& file)
  {
    std::string filepath = file.string();
    std::optional< std::string_view > ext_opt = get_extension(filepath);

    if(!ext_opt.has_value()) { return false; }

    std::string_view file_ext = ext_opt.value();
    if(file_ext == "o" || file_ext == "obj") { return true; }
    return false;
  }
		
	[[nodiscard]]
  inline std::vector< fs::path > get_src_files(const fs::path& src_dir)
  {
    std::vector< fs::path > src_files;

    for(const auto& entry: fs::recursive_directory_iterator(src_dir))
    {
      if(!entry.is_regular_file()) { continue; }
      
      fs::path file_path = entry.path();
      if(file_path.empty()) { continue; }
      
      if(is_src_file(file_path))
      {
        src_files.push_back(file_path);
      }
      else if(is_header_file(file_path))
      {
        std::optional< std::string_view > fn_opt  = get_filename(file_path.string());
        assert(fn_opt.has_value());

        std::string_view filename = fn_opt.value();
        std::cerr << "[WARNING] Header file in src directory: "
          << filename << "\n";
      }
    }
    return src_files;
  }
  
  [[nodiscard]] 
  inline std::vector< fs::path > get_header_files(const fs::path& include_dir)
  {
    std::vector< fs::path > header_files;

    for(const auto& entry: fs::recursive_directory_iterator(include_dir))
    {
      if(!entry.is_regular_file()) { continue; }

      fs::path file_path = entry.path();
      if(file_path.empty()) { continue; }
      
      if(is_header_file(file_path))
      {
        header_files.push_back(file_path);
      }
      else if(is_src_file(file_path))
      {
        std::optional< std::string_view > fn_opt = get_filename(file_path.string());
        assert(fn_opt.has_value());

        std::string_view filename = fn_opt.value();
        std::cerr << "[WARNING] Source file in include/: "
          << filename << "\n";
      }
    }
    return header_files;
  }

  [[nodiscard]]
  inline std::vector< fs::path > get_obj_files(const fs::path& root)
  { 
    std::vector< fs::path > obj_files;
    for(const auto& entry: fs::recursive_directory_iterator(root))
    {
      if(!entry.is_regular_file()) { continue; }

      fs::path cur = entry.path();
      if(cur.empty()) { continue; }
      
      if(is_obj_file(cur))
      {
        obj_files.push_back(cur);
      }
    }
    return obj_files;
  }

  inline fs::path get_full_path(std::string_view filename)
  {
    fs::path p(filename);
    return fs::absolute(p);
  }
}
