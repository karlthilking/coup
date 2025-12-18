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
		bool execute() { return true; }
	};

	
	using command = std::variant< run_command, build_command, clean_command >;
	
	bool execute(const command& cmd)
	{
		std::visit([](const auto& c){ c.execute(); }, cmd);
	}

}
