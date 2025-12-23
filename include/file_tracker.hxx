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
  std::optional< fs::path > find_root(const fs::path& p = fs::current_path())
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

  std::optional< fs::path > get_src(const fs::path& root)
  {
    if(fs::exists(root / "src"))
    {
      return fs::path(root / "src");
    }
    return std::nullopt;
  }

  std::optional< fs::path > get_include(const fs::path& root)
  {
    if(fs::exists(root / "include"))
    {
      return fs::path(root / "include");
    }
    return std::nullopt;
  }

  [[nodiscard]] constexpr bool is_src_file(const fs::path& file)
  {
    assert(fs::exists(file));
    assert(!file.empty());
    std::string file_ext = get_extension(file.string());

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
  
  [[nodiscard]] constexpr bool is_header_file(const fs::path& file)
  {
    assert(fs::exists(file));
    assert(!file.empty());
    std::string file_ext = get_extension(file.string());

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
  std::vector< fs::path > get_src_files(const fs::path& src_dir)
  {
    std::vector< fs::path > src_files;

    for(const auto& entry: fs::recusrive_directory_iterator(src_dir))
    {
      if(!entry.is_regular_file()) { continue; }
      
      fs::path cur = entry.path();
      if(cur.empty()) { continue; }

      std::optional< std::string_view > fn_opt = get_filename(cur.string());
      assert(fn_opt.has_value());
      std::string_view filename = fn_opt.value();
      
      std::optional< std::string_view > ext_opt = get_extension(filename);
      if(!ext_opt.has_value()) { continue; }

      std::string_view ext = ext_opt.value();
      if(is_source_file(ext))
      {
        src_files.push_back(cur);
      }
      else if(is_header_file(ext))
      {
        std::cerr << "[WARNING] Header file in src/: "
          << filename << "\n";
    }
    return src_files;
  }
  
  [[nodiscard]] 
  std::vector< fs::path > get_header_files(const fs::path& include_dir)
  {
    std::vector< fs::path > header_files;

    for(const auto& entry: fs::recursive_directory_iterator(include_dir))
    {
      if(!entry.is_regular_file()) { continue; }

      fs::path cur = entry.path();
      if(cur.empty()) { continue; }
      
      std::optional< std::string_view > fn_opt = get_filename(cur.string());
      assert(fn_opt.has_value());
      std::string_view filename = fn_opt.value();
     
      std::optional< std::string_view > ext_opt = get_extension(fn);
      if(!ext_opt.has_value()) { continue; }

      std::string_view ext = ext_opt.value();
      if(is_header_file(ext))
      {
        header_files.push_back(cur);
      }
      else if(is_source_file(ext))
      {
        std::cerr << "[WARNING] Source file in include/: "
          << filename << "\n";
      }
    }
    return header_files;
  }
}
