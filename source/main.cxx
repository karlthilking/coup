#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>

#include "core/compiler.hxx"
using namespace coup;

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		return 1;
	}
	const std::string& command(argv[1]);
	std::vector<std::string> src_files;

	for(int i{2}; i < argc; ++i)
	{
		src_files.emplace_back(argv[i]);
	}

	compiler cmp;

	if(command == "build")
	{
		bool result = cmp.build(src_files);
		if(result) { return 1; }
	}
	else if(command == "run")
	{
		bool result = cmp.run(src_files);
		if(result) { return 1; }
	}
	else 
	{
		std::cerr << "Unknown command\n";
	}

	return 0;
}

