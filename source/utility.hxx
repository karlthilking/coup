#pragma once

#include <cstdlib> // std::system 
#include <cassert> // assert
#include <string>
#include <vector>
#include <iostream> // std::cout std::cerr

#include "parse.hxx" // convert_to_obj_file(s)
#include "config.hxx" // CPP macro

namespace coup {
	constinit static const char* cpp_version = CPP; // c++17, c++20, etc

	int exec_sys_call(const std::string& command)
	{
		int result = std::system(command.c_str());
		return result;
	}

	void print_usage()
	{
		std::cout <<
			"To build your project use [ coup build <source1> <source2> ... ]\n
			 To run your project use [ coup run <source1> <source2> ... ]\n
			 To clean your project use [ coup clean <source1> <source2> ...]\n";
	}

	std::string create_compile_command(const std::string& source_file)
	{
		std::string compile_cmd("g++ -std=");
		compile_cmd += cpp_version + " -c " + source_file + " -o ";
		compile_cmd += convert_to_obj_file(source_file);
		return compile_cmd;
	}
	
	std::string create_link_command(const std::vector< std::string >& object_files)
	{
		std::string link_cmd("g++ -std=");
		link_cmd += cpp_version;

		for(const std::string& obj: object_files)
		{
			link_cmd += " " + obj;
		}

		link_cmd += " -o program";
		return link_cmd;
	}

	std::string create_run_command() { return std::string("./program"); }

	bool compile(const std::vector< std::string >& source_files)
	{
		for(const std::string& source: source_files)
		{
			const std::string& compile_command(create_compile_command(source));
			int compilation_result = exec_sys_call(compile_command);

			if(compilation_result)
			{
				std::cerr << "Failed to compile " << source << "\n"; return false;
			}
		}
		return true;
	}

	bool link(const std::vector< std::string >& object_files)
	{
		const std::string& link_command(create_link_command(object_files));

		int linking_result = exec_sys_call(link_command);

		if(linking_result)
		{
			std::cerr << "Failed to linked object files\n"; return false;
		}
		return true;
	}

	bool run_executable()
	{
		const std::string& run_command(create_run_command());

		int execution_result = exec_sys_call(run_command);

		if(execution_result) { std::cerr << "Runtime error\n"; return false; }
		return true;
	}

	bool build(const std::vector< std::string >& source_files)
	{
		bool compilation_result = compile(source_files);
		assert(compilation_result == true);
	
		convert_to_obj_files(source_files);

		bool link_result = link(source_files); // source files now object files
		assert(link_result == true);
		
		std::cout << "Built all source files successfully\n";
		return true;
	}

	bool run(const std::vector< std::string >& source_files)
	{
		bool build_result = build(source_files);
		assert(build_result == true);

		bool run_result = run_executable();
		assert(run_result == true);

		std::cout << "Built all source files and ran executable\n";
		return true;
	}
	
	bool clean() { return true; }
}

