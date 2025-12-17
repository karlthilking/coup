#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

#include "compiler.hxx"


namespace coup {
	int compiler::execute_command(const std::string& command)
	{
		int result = std::system(command.c_str());
		return result;
	}

	std::string compiler::create_build_command(const std::vector<std::string>& src_files)
	{
		std::string command("g++ -std=" + cpp_version);
		for(const std::string& src: src_files)
		{
			command += " " + src;
		}
		command += " -o project";
		
		#ifdef DEBUG
			std::cout << "(DEBUGGING COMMAND OUTPUT) " << command << "\n";
		#endif
		return command;
	}

	std::string compiler::create_run_command()
	{
		return std::string("./project");
	}

	bool compiler::build(const std::vector<std::string>& src_files)
	{
		const std::string& command(create_build_command(src_files));
		int result = execute_command(command);

		if(result) { std::cerr << "Failed to build\n"; return false; }
		else { return true; }
	}

	bool compiler::run(const std::vector<std::string>& src_files)
	{
		const std::string& build_command(create_build_command(src_files));
		int build_result = execute_command(build_command);

		if(build_result)
		{ std::cerr << "Failed to build source files\n"; return false; }
		
		std::string run_command(create_run_command());
		int run_result = execute_command(run_command);

		if(run_result) { std::cerr << "Failed to run\n"; return false; }
		else { return true; }
	}
}


