#include "../include/coup_system.hxx"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <ranges>
#include <string>
#include <vector>

#include "../include/coup_filesystem.hxx"
#include "../include/coup_logger.hxx"
#include "../include/coup_project.hxx"
#include "../include/coup_system.hxx"

namespace fs = std::filesystem;
namespace coup
{

// executes a system call/commnad, returns true if successful, false otherwise
bool execute_system_call(const char *command)
{
	int result = std::system(command);
	return result == 0;
}

// composes a compile command for a given source file
std::string make_compile_command(const fs::path &src_file)
{
	assert(fs::exists(src_file));

	std::string src_name = src_file.string();
	assert(!src_name.empty());

	std::string compile_command = "g++ -std=c++20 -c " + src_name;
	return compile_command;
}

// composes a link command for a given list of object files
std::string make_link_command(const std::vector<fs::path> &obj_files)
{
	assert(!obj_files.empty());
	std::string link_command = "g++ -std=c++20 -o coup_exec ";

#if (__cpp_lib_ranges >= 201911L)
	std::ranges::for_each(obj_files, [&](const fs::path &obj)
						  { link_command += obj.string() + " "; });
#else
	std::for_each(begin(obj_files), end(obj_files), [&](const fs::path &obj)
				  { link_command += obj.string() + " "; });
#endif
	return link_command;
}

std::string
make_compile_and_link_command(const std::vector<fs::path> &src_files)
{
	assert(!src_files.empty());
	std::string compile_link_command = "g++ -o prog ";

#if (__cpp_lib_ranges >= 201911L)
	std::ranges::for_each(src_files, [&](const fs::path &src)
						  { compile_link_command += src.string() + " "; });
#else
	std::for_each(begin(src_files), end(src_files), [&](const fs::path &src)
				  { compile_link_command += src.string() + " "; });
#endif
	return compile_link_command;
}

// composes a run command for a given executable
std::string make_run_command(const fs::path &exec_file)
{
	assert(fs::exists(exec_file));
	return exec_file.string();
}

// composes a -MMD command for a given source file
std::string make_mm_command(const fs::path &src_file, const fs::path &dep_file)
{
	std::string mm_command =
		"g++ -MM " + src_file.string() + " > " + dep_file.string();
	return mm_command;
}

// composes a system command with a command and file (e.g. cat main.cpp)
std::string make_system_command(const std::string &command,
								const fs::path &file)
{
	return command + " " + file.string();
}

// obtain compile command for a single source file and execute
// returns true if successful, false otherwise
bool compile(const fs::path &src_file)
{
	std::string compile_command = make_compile_command(src_file);
	bool result = execute_system_call(compile_command.c_str());
	return result;
}

// compile each source file in a collection
bool compile(const std::vector<fs::path> &src_files)
{
	assert(!src_files.empty());
	bool success = true;

	for (const fs::path &src : src_files)
	{
		assert(fs::exists(src));
		if (!compile(src))
		{
			success = false;
			break;
		}
	}
	return success;
}

// obtain command to link object files and execute linkage
// return true if successful, false otherwise
bool link(const std::vector<fs::path> &obj_files)
{
	std::string link_command = make_link_command(obj_files);
	bool result = execute_system_call(link_command.c_str());
	return result;
}

// obtain command to compile and link source files in one step
// return true if successful, false otherwise
bool compile_and_link(const std::vector<fs::path> &src_files)
{
	std::string compile_link_command = make_compile_and_link_command(src_files);
	bool result = execute_system_call(compile_link_command.c_str());
	return result;
}

// obtain command to run a given executable
// return true if successful, false otherwise
bool run(const fs::path &exec_file)
{
	std::string run_command = make_run_command(exec_file);
	bool result = execute_system_call(run_command.c_str());
	return result;
}

// obtain command to remove a given file
// return true if command executes and file is removed, false otherwise
bool remove_file(const fs::path &file)
{
	assert(fs::exists(file));

	std::string rm_command = make_system_command("rm", file);
	bool result = execute_system_call(rm_command.c_str());
	return result && !fs::exists(file);
}

// obtain command to remove a given directory
// returns true if command executes and directory is removed, false otherwise
bool remove_directory(const fs::path &dir)
{
	assert(fs::exists(dir));

	std::string rmdir_command = make_system_command("rmdir", dir);
	bool result = execute_system_call(rmdir_command.c_str());
	return result && !fs::exists(dir);
}

// obtain command to create a given directory
// returns true if command executes and directory is created, false otherwise
bool make_directory(const fs::path &dir)
{
	std::string mkdir_command = make_system_command("mkdir", dir);
	bool result = execute_system_call(mkdir_command.c_str());
	return result && fs::exists(dir);
}

// obtain command to create dependency file for a given source file
// parse dependecy file to obtain all individual dependencies
// return a vector of filenames representing dependencies
std::vector<std::string> get_dependencies(const fs::path &src_file)
{
	auto dep_file = make_dep_file(src_file);
	std::string mm_command = make_mm_command(src_file, dep_file);

	bool command_result = execute_system_call(mm_command.c_str());
	assert(command_result == true);

	std::vector<std::string> dependencies = parse_dependency_file(dep_file);
	return dependencies;
}

} // namespace coup
