#pragma once

#include <string>
#include <vector>

namespace coup {
	class compiler {
	private:
		int execute_command(const std::string& cmd);

		std::string create_command(const std::string& src, const std::string& out, const std::vector<std::string>& flags = {});

	public:
		bool compile(const std::string& src, const std::string& out, const std::vector<std::string>& flags = {});

		bool link(const std::vector<std::string>& objs, const std::string& out);

	};
}



