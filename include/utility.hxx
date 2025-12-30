#pragma once

#include <string_view>
#include <cstdlib> // std::system 
#include <cassert> // assert
#include <string>
#include <vector>
#include <iostream> // std::cout std::cerr
#include <thread>

#include "parser.hxx" // convert_to_obj_file(s)
#include "config.hxx" // CPP macro

namespace coup {

	constinit static const char* cpp_version = CPP; // c++17, c++20, etc

	inline int exec_sys_call(const std::string& command)
	{
		int result = std::system(command.c_str());
		return result;
	}

	inline void print_usage()
	{
		std::cout << "To run use coup run\n"
			<< "To build use coup build\n"
			<< "To clean use coup clean\n";
	}

	inline std::string create_compile_command(std::string_view source_file)
	{
		std::string compile_cmd = "g++ -std=";
		compile_cmd += cpp_version;
		compile_cmd += " -c ";
		compile_cmd += source_file;
		compile_cmd += " -o ";
		compile_cmd += create_obj_file(source_file);
		return compile_cmd;
	}
	
	inline std::string create_link_command(const std::vector< std::string >& object_files)
	{
		std::string link_cmd = "g++ -std=";
		link_cmd += cpp_version;

		for(const std::string& obj: object_files)
		{
			link_cmd += " " + obj;
		}

		link_cmd += " -o program";
		return link_cmd;
	}

	inline std::string create_run_command() { return "./program"; }

	inline bool compile(const std::vector< std::string >& source_files)
	{
    bool success = true;
    std::vector< std::thread > compile_threads;
		for(const std::string& source: source_files)
		{ 
      compile_threads.emplace_back([&]
      {
        const std::string& compile_command(create_compile_command(source));
        int compilation_result = exec_sys_call(compile_command);

        if(compilation_result)
        {
          std::cerr << "Failed to compile " << source << "\n";
          success = false;
        }
      });
    }
    for(std::thread& th: compile_threads) { th.join(); }
		return success;
	}

	inline bool link(const std::vector< std::string >& object_files)
	{
		const std::string& link_command(create_link_command(object_files));

		int linking_result = exec_sys_call(link_command);

		if(linking_result)
		{
			std::cerr << "Failed to linked object files\n"; return false;
		}
		return true;
	}

	inline bool run_executable()
	{
		const std::string& run_command(create_run_command());

		int execution_result = exec_sys_call(run_command);

		if(execution_result) { std::cerr << "Runtime error\n"; return false; }
		return true;
	}

	inline bool build(const std::vector< std::string >& source_files)
	{
		bool compilation_result = compile(source_files);
		assert(compilation_result == true);
	
		std::vector< std::string > obj_files = create_obj_files(source_files);

		bool link_result = link(obj_files); 
		assert(link_result == true);
		
		std::cout << "Built all source files successfully\n";
		return true;
	}

	inline bool run(const std::vector< std::string >& source_files)
	{
		bool build_result = build(source_files);
		assert(build_result == true);

		bool run_result = run_executable();
		assert(run_result == true);

		std::cout << "Built all source files and ran executable\n";
		return true;
	}
	
	inline bool clean()
  {
    return true;
  }
}

