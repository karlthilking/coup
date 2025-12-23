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
	if(argc < 2)
	{
		print_usage();
		return 1;
	}

	const char* cmd_str = argv[1];
	std::vector< std::string > args;

	for(int i{2}; i < argc; ++i)
	{
		args.emplace_back(argv[i]);
	}
	
	command_type cmd = create_command(cmd_str);
	bool success = execute_cmd(cmd, args);

	if(!success)
	{
		std::cerr << "Error executing command: " << cmd_str << "\n";
		return 1;
	}

	return 0;
}

