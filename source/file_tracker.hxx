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
				if(fs::exists(current / "src") || fs::exists(current / "source"))
				{
					return current;
				}
				current = current.parent_path();
			} while(current.parent_path() != current);

			return std::nullopt;
		}

		bool is_source_file(std::string_view file_ext) const noexcept
		{
			if(file_ext == "cpp" || file_ext == "cc" || file_ext == "cxx")
			{
				return true;
			}
			return false;
		}

		bool is_header_file(std::string_view file_ext) const noexcept
		{
			if(file_ext == "h" || file_ext == "hpp" || file_ext == "hxx")
			{
				return true;
			}
			return false;
		}

		void initialize_files() noexcept
		{
			assert(fs::exists(root_path) && !root_path.empty());
		
			for(const auto& entry: fs::recursive_directory_iterator(root_path))
			{
				if(!entry.is_regular_file()) { continue;}
				
				std::string filename = entry.path().string();
				std::optional< std::string_view > ex = get_extension(filename);

				if(!ex.has_value()) { continue;}
				std::string_view ext = ex.value();

				if(is_source_file(ext)) { source_files.push_back(entry);}
				if(is_header_file(ext)) { header_files.push_back(entry);}
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
