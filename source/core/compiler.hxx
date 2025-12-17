#pragma once

#include <string>
#include <vector>
#include "config.hxx"

namespace coup {
	class compiler {
	private:
		std::string cpp_version = CPP;
		int execute_command(const std::string& command);

		std::string create_build_command(const std::vector<std::string>& src_files);

		std::string create_run_command();

	public:
		bool build(const std::vector<std::string>& src_files);

		bool run(const std::vector<std::string>& src_files);
	};
}



