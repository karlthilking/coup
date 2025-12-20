#include <string>
#include <optional>


namespace coup {
	
	std::optional< std::string > extract_header(const std::string& line) 
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
