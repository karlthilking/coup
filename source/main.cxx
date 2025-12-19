#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>

#include "utility.hxx" // print_usage
#include "commands.hxx" // build_command, run_command, clean_command, command, execute
												// create_command

using namespace coup;

int main(int argc, char** argv)
{
	if(argv < 2)
	{
		print_usage();
		return 1;
	}

	const char* cmd = argv[2];
	std::vector< std::string > args;

	for(int i{3}; i < argc; ++i)
	{
		args.emplace_back(argv[i]);
	}
	
	command exec_cmd = create_command(cmd);

	bool success = execute(exec_cmd, args);

	if(!success)
	{
		std::cerr << "Error executing command: " << command << "\n";
		return 1;
	}

	return 0;
}

