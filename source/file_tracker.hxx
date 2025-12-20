#include <filesystem> // path, last_write_time
#include <fstream> // ifstream
#include <vector>

using fs = std::filesystem; 

namespace coup {
private:
	std::vector< fs::path > file_paths;
	std::unorderd_map< fs::path, std::vector< fs::path >> file_adj_list;
	bool topo_sorted = false;
public:
	
	std::vector< fs::path > get_dependencies(const fs::path pth)
	{
		assert(!pth.empty());

		std::ifstream input(pth);
		std::string line;

		while(std::getline(input, line))
		{


	}

	void build_file_adj_list() noexcept
	{
		
	}

	void topological_sort_files() noexcept
	{
		// determine how many dependencies each file has
		std::unordered_map< fs::path, int > file_dependencies;
	}
	
	std::vector< fs::path > get_requires_recompilation() const noexcept
	{
		assert(topo_sorted == true);

		for(const fs::path& pth: file_paths)
		{
			
	}

	fs::file_time_type get_last_update(const fs::path& pth) const noexcept
	{
		return fs::last_write_time(pth);
	}
}
