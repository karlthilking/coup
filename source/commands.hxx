#pragma once

#include <variant> // variant, visit
#include <utility> // move

#include "utility.hxx" // build(), run(), clean()

namespace coup {
	
	class build_command {
	public:
		bool execute(const std::vector< std::string >& source_files)
		{
			return build(source_files);
		}
	};

	class run_command {
	public:
		bool execute(const std::vector< std::string >& source_files)
		{
			return run(source_files);
		}
	};

	class clean_command {
	public:
		bool execute(const std::vector< std::string >& source_files)
		{ 
			return true;
		}
	};

	
	using command = std::variant< run_command, build_command, clean_command >;
	
	bool execute(const command& cmd, const std::vector< std::string >& args)
	{
		std::visit([&args](const auto& c){ return c.execute(args); }, cmd);
	}
	
	command create_command(const char* cmd)
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
			std::cerr << "Invalid command used\n";
		}
	}

}
