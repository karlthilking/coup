#pragma once

#include <string_view>
#include <cstdlib> // std::system 
#include <cassert> // assert
#include <string>
#include <vector>
#include <iostream> // std::cout std::cerr
#include <thread>

#include "parser.hxx" // sv_cstr, create_obj_file, create_obj_files
#include "config.hxx" // CPP macro

namespace coup {

	constinit static const char* cpp_version = CPP; // holds cpp version as a const char pointer: (e.g. c++17, c++20, etc)
  
  // executes a system call given by a command string
  // converts string to a const char pointer
  // returns result code (0 == success, else failure)
	inline int system_call(const std::string& command)
	{
		int result = std::system(command.c_str());
		return result;
	}
  
  // executes a system call given a command string view
  // converts string view to a const char pointer
  // returns result code
  inline int system_call(std::string_view command)
  {
    int result = std::system(sv_cstr(command));
    return result;
  }
  
  // prints system usage to user
	inline void print_usage()
	{
		std::cout << "To run use coup run\n"
			<< "To build use coup build\n"
			<< "To clean use coup clean\n";
	}
  
  // creates compile command for a given source file string
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
	
  // creates link command for a collection of object files
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
  
  // creates command to run project executable
	inline std::string create_run_command()
  {
    return "./program";
  }

  // compiles a collection of source files with multiple threads
  // each thread creates a compile command string and executes a system call to compile a source file
  // exists and returns false if any source file compilation fails, otherwise returns true on success
	inline bool compile(const std::vector< std::string >& source_files)
	{
    bool success = true;
    std::vector< std::thread > compile_threads;

		for(const std::string& source: source_files)
		{ 
      if(!success) { break; }
      compile_threads.emplace_back([&]
      {
        const std::string& compile_command(create_compile_command(source));
        int compilation_result = system_call(compile_command);

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
  
  // obtains string command to link a collection of object files and executes a system call to link
  // return true if linking is successful, false on failure
	inline bool link(const std::vector< std::string >& object_files)
	{
		const std::string& link_command(create_link_command(object_files));

		int linking_result = system_call(link_command);

		if(linking_result)
		{
			std::cerr << "Failed to linked object files\n"; return false;
		}
		return true;
	}
  
  // obtains string command to run the project executable and executes system call to run
  // returns true if the executable runs (does not access runtime errors only execution), false on failure 
	inline bool run_executable()
	{
		const std::string& run_command(create_run_command());

		int execution_result = system_call(run_command);

		if(execution_result) { std::cerr << "Runtime error\n"; return false; }
		return true;
	}
  
  // compiles a collection of source files and links resulting object files after compiling
  // returns true if compiling and linking are successful, false otherwise
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
  
  // compiles source files, links resulting object files, and run output executable
  // returns true if each step is successful, false otherwise
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
    return true; // TODO
  }
}

