#include <string>
#include <optional>
#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;

namespace coup {
	
	std::optional< std::string_view > extract_header(std::string_view line) 
	{
		if(line.empty()) { return {}; }
		if(line.find("#include") == line.end()) { return {}; }

		size_t l = line.find('"') + 1;
		if(l == std::string::npos) { return {}; }
		size_t r = line.rfind('"');
		if(r == std::string::npos) { return {}; }

		return line.substr(l, r - l);
	}
	
	
}
