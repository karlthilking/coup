#pragma once

#include <filesystem> // path, last_write_time, current_path, parent_path, exists
#include <fstream> // ifstream
#include <vector>
#include <string_view>
#include <optional>
#include <cassert> // assert
#include <stdexcept> // runtime_error

#include "regex.hxx" // extract_header
#include "parser.hxx" // get_stem, get_extension

namespace fs = std::filesystem; 

namespace coup {

	class file_tracker {

	private:
		fs::path root_path;
		std::vector< fs::path > source_files;
		std::vector< fs::path > header_files;
	
		std::optional< fs::path > find_root(fs::path p = fs::current_path())
		{
			fs::path current = fs::absolute(p);
			assert(fs::exists(current));

			do
			{
				if(fs::exists(current / "src") || fs::exists(current / "source") ||
					 fs::exists(current / "include"))
				{
					return current;
				}
				current = current.parent_path();
			} while(current.parent_path() != current);

			return std::nullopt;
		}

		bool is_source_file(std::string_view file_ext) const noexcept
		{
			assert(!file_ext.empty());

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

		bool is_header_file(std::string_view file_ext) const noexcept
		{
			assert(!file_ext.empty());

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
		
		void find_src_files(const fs::path src_dir) noexcept
		{
			for(const auto& entry: fs::recursive_directory_iterator(src_dir))
			{
				if(!entry.is_regular_file()) { continue;}

				std::string src_name = entry.path().string();
				std::optional< std::string_view > opt = get_extension(src_name);

				if(!opt.has_value()) { continue;}
				std::string_view src_ext = opt.value();

				if(is_source_file(src_ext)) { source_files.push_back(entry);}
				if(is_header_file(src_ext))
				{
					std::cerr << "[WARNING] Header file in src/: "
						<< src_name << "\n";
					header_files.push_back(entry);
				}
			}
		}

		void find_header_files(const fs::path& include_dir) noexcept
		{
			for(const auto& entry: fs::recursive_directory_iterator(include_dir))
			{
				if(!entry.is_regular_file()) { continue;}

				std::string header_name = entry.path().string();
				std::optional< std::string_view > opt = get_extension(header_name);

				if(!opt.has_value()) { continue;}
				std::string_view header_ext = opt.value();

				if(is_header_file(header_ext)) { header_files.push_back(entry);}
				if(is_source_file(header_ext))
				{
					std::cerr << "Source file in include/: "
						<< header_name << "\n";
					source_files.push_back(entry);
				}
			}
		}

		void initialize_files() noexcept
		{
			assert(fs::exists(root_path) && !root_path.empty());
			bool src_dir_exists = false;
			bool include_dir_exists = false;

			if(fs::exists(root_path / "src"))
			{
				fs::path src_dir = root_path / "src";
				find_src_files(src_dir);
				src_dir_exists = true;
			}
			else if(fs::exists(root_path / "source"))
			{
				fs::path src_dir = root_path / "source";
				find_src_files(src_dir);
				src_dir_exists = true;
			}
			
			if(fs::exists(root_path / "include"))
			{
				fs::path header_dir = root_path / "include";
				find_header_files(header_dir);
				include_dir_exists = true;
			}
			
			if(!src_dir_exists && !include_dir_exists)
			{
				throw std::runtime_error("No src/source or include directories\n");
			}
		}	

	public:
		file_tracker() 
		{
			std::optional< fs::path > root = find_root();
			
			if(!root.has_value())
			{
				throw std::runtime_error("Could not identify a root directory\n");
			}

			root_path = root.value();
			initialize_files();
		}
		
		fs::path get_root() const noexcept { return root_path; }
		
		std::vector< fs::path > get_source_files() const noexcept
		{
			return source_files;
		}

		std::vector< fs::path > get_header_files() const noexcept
		{
			return header_files;
		}

		/*
		std::vector< fs::path > get_dependencies(const fs::path& pth)
		{
			std::vector< fs::path > dependencies;
			std::ifstream input(pth);
			std::string line;

			while(std::getline(input, line))
			{
				std::optional< std::string_view > h = extract_header(line);
				if(h.has_value())
				{
					std::optional< fs::path > src = get_source(h.value());
					if(src.has_value()) { dependencies.push_back(src.value()); }
				}
			}
			if(dependencies.empty()) { return {};}
			else { return dependencies;}
		}
		
		
		void build_file_adj_list() noexcept
		{
		
		}

		void topological_sort_files() noexcept
		{
		}
	
		std::vector< fs::path > get_requires_recompilation() const noexcept
		{
			
		}

		fs::file_time_type get_last_update(const fs::path& pth) const noexcept
		{
		}
		*/
	};
}
