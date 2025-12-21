#pragma once

#include <variant> // variant, visit
#include <utility> // move
#include <type_traits> // is_same_v
#include <stdexcept> // runtime_error
#include <cassert> // assert
#include <string_view>

#include "utility.hxx" // build(), run(), clean()

namespace coup {
	
	class build_command {
	public:
		bool execute(const std::vector< std::string >& source_files) const noexcept
		{
			return build(source_files);
		}
	};

	class run_command {
	public:
		bool execute(const std::vector< std::string >& source_files) const noexcept
		{
			return run(source_files);
		}
	};

	class clean_command {
	public:
		bool execute([[maybe_unused]] const std::vector< std::string >& source_files) const noexcept
		{ 
			return true;
		}
	};
	
	using command_type = std::variant< run_command, build_command, clean_command >;
	
	bool execute_cmd(const command_type& cmd, const std::vector< std::string >& args)
	{
		assert(!args.empty());
		return std::visit([&args](const auto& c)
		{
			return c.execute(args);
		}, cmd);
	}

	command_type create_command(std::string_view cmd)
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
