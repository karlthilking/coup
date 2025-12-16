#include <vector>
#include <string>
#include <cstdlib>

#include <coup/source/config.hxx>
#include <coup/source/compiler.hxx>

namespace coup {
	compiler::execute_command(const std::string& cmd)
	{
		int result = std::system(cmd);
		return result;
	}

	compiler::create_compile_command(const std::string& src, const std::string& out, const std::vector<std::string>& flags = {})
	{
		std::string cmd = COMPILER + " std=" + CPP + " -c " + src + " -o " + out;
		for(const std::string& flag: flags)
		{
			cmd += " " + flag;
		}
		return cmd;
	}

	compiler::create_link_command(const std::vector<std::string>& objs, const std::string& out)
	{
		std::string cmd = COMPILER + " -o";
		
		for(const std::string& o: objs)
		{
			cmd += " " + o;
		}
		return cmd;
	}

	bool compile(const std::string& src, constd std::string& out, const std::vector<std::string>& flags = {})
	{
		std::string cmd(create_compiler_command(src, out, ver, flags));
		int result = execute_command(cmd);

		if(result)
		{
			std::cerr << "Failed to compile " << src << "\n";
			return false;
		}
		else
		{
			return true;
		}
	}

	bool link(const std::vector<std::string>& objs, const std::string& out)
	{
		std::string cmd(create_link_command(objs, out));
		int result = execute_command(cmd);

		if(result)
		{
			std::cerr << "Failed to link\n";
			return false;
		}
		else
		{
			return true;
		}
	}
}


