#include <filesystem> // path, last_write_time
#include <fstream> // ifstream
#include <vector>
#include <string_view>
#include <optional>

#include "regex.hpp" // extract_header

using fs = std::filesystem; 

namespace coup {

	class file_tracker {

	private:
		std::vector< fs::path > file_paths;
		
		bool update()
		{
			assert(!file_paths.empty());
		}

		std::string_view remove_extension(std::string_view filename)
		{
			size_t extension_pos = filename.rfind('.');
			assert(extension_pos != std::string_view::npos);

			return filename.substr(0, extension_pos);
		}

		std::optional< fs::path > get_source(std::string_view header)
		{
			std::string_view h_name = remove_extension(header);

			for(const auto& entry: file_paths)
			{
				std::string full = entry.path().filename().string();
				if(full == header) { continue; }

				std::string_view f_name = remove_extension(full);
				if(f_name == h_name) { return entry; }
			}
			return std::nullopt;
		}

	public:
	
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
			if(dependencies.empty()) { return {}; }
			else return dependencies;
		}

		void build_file_adj_list() noexcept
		{
		
		}

		void topological_sort_files() noexcept
		{
			std::unordered_map< fs::path, int > file_dependencies;
		}
	
		std::vector< fs::path > get_requires_recompilation() const noexcept
		{
			
		}

		fs::file_time_type get_last_update(const fs::path& pth) const noexcept
		{
			return fs::last_write_time(pth);
		}
	}
}
