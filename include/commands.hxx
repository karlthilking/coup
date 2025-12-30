#pragma once

#include <variant> // variant, visit
#include <utility> // move
#include <stdexcept> // runtime_error
#include <cassert> // assert
#include <string_view>
#include <filesystem>
#include <vector>
#include <thread>

#include "../include/utility.hxx" // build, run, clean
#include "../include/project.hxx" // coup_project, coup_project::create_project

namespace coup {
	
	class build_command {
	public:
		bool execute() const 
		{
      coup_project proj = coup_project::create_project();
      return true;
    }
	};

	class run_command {
	public:
		bool execute() const 
		{
      coup_project proj = coup_project::create_project();
      return true;
		}
	};

	class clean_command {
	public:
		bool execute() const noexcept
		{ 
			return true;
		}
	};
	
	using command_type = std::variant< run_command, build_command, clean_command >;
	
  // dispatch command execution to the correct type
	inline bool execute_cmd(const command_type& cmd)
	{
		return std::visit([](const auto& c)
		{
			return c.execute();
		}, cmd);
	}
  
  // returns the correct command type based on user argument
	inline command_type create_command(std::string_view cmd)
	{
		if(cmd == "build")
		{
			return build_command{};
		}
		else if(cmd == "run")
		{
			return run_command{};
		}
		else if(cmd == "clean")
		{
			return clean_command{};
		}
		else
		{
			print_usage();
			throw std::runtime_error("invalid command");
		}
	}

}
