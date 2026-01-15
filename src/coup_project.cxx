/* coup_project.cxx */
#include "../include/coup_project.hxx"

#include <algorithm>
#include <array>
#include <cassert>
#include <filesystem>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../include/coup_filesystem.hxx"
#include "../include/coup_logger.hxx"
#include "../include/coup_system.hxx"

namespace fs = std::filesystem;
namespace coup
{

coup_project::coup_project(const std::vector<fs::path>& source_files_,
                           const std::vector<fs::path>& object_files_,
                           const std::vector<fs::path>& source_directories_,
                           const fs::path& build_directory_,
                           const fs::path& executable_path_,
                           const coup_json& coup_config_)
	: source_files(source_files_),
      object_files(object_files_),
      source_directories(source_directories_),
      build_directory(build_directory_),
      executable_path(executable_path_)
      coup_config(coup_config_)
{}

coup_project::coup_project(std::vector<fs::path>&& source_files_,
                           std::vector<fs::path>&& object_files_,
                           std::vector<fs::path>&& source_directories_,
                           fs::path&& build_directory_, 
                           fs::path&& executable_path_,
                           coup_json&& coup_config_) noexcept
    : source_files(std::move(source_files_)),
      object_files(std::move(object_files_)),
      source_directories(std::move(source_directories_)),
      build_directory(std::move(build_directory_)),
      executable_path(std::move(executable_path_))
      coup_config(std::move(coup_config_))
{}

// static function: sets up and returns initialized coup_project
// Find root directory
// Find coup_config.json
// Find source directories
// Find build directory (okay if it doesn't actually exist yet)
// Find source files
// Find object files (okay if they don't actually exist yet)
// Find executable path (okay if it doesn't actually exist yet)
coup_project coup_project::make_project()
{
    fs::path root, coup_config, build_directory, executable_path;
    std::vector<fs::path> source_directories, source_files, object_files;
    coup_json coup_config;

	try {
        root = get_root_dir();
    } catch (const std::runtime_error& e) {
        // log error and exit
    }
    
    coup_config_path = root / "coup_config.json";
    if (!fs::exists(coup_config_path)) {
        throw std::runtime_error("No coup_config.json found");
    }
    coup_config = coup_json(coup_config_path);
    source_directories = config.get_source_directories();
    build_directory = config.get_build_directory();

    for (const fs::path& source_directory : source_directories) {
        assert(fs::exists(source_directory);
        std::vector<fs::path> new_sources = find_src_files(source_directory);
        source_files.insert(source_files.back(), new_sources.begin(), 
                            new_sources.end());
    }
    object_files = find_obj_files(build_directory);
    executable_path = build_directory / config.get_executable();

    return coup_project(std::move(source_files), 
                        std::move(object_files),
                        std::move(source_directories),
                        std::move(build_directory),
                        std::move(executable_path),
                        std::move(coup_config));
}


// Executes build step with multiple parallel workers
// Each worker completes the following task:
//      - Retrieves a source file
//      - Compiles source and prints compilation log
//      - Stores a new object file for the compiled source
// If the function returns a string, an error has occurred and the string
// will contain a description of the error
// Otherwise, std::nullopt will be returned

std::optional<std::string> coup_project::execute_build(bool verbose) noexcept
{
    std::vector<fs::path> source_files;
    for (const fs::path& source_directory : source_directories) {
        std::vector<fs::path> new_source_files = find_src_files(source_directory);
        source_files.insert(source_files.end(), 
                            std::make_move_iterator(new_source_files.begin()),
                            std::make_move_iterator(new_source_files.end());
    }

    // Critical sections needed locking:
    //      - removing from source_files vector
    //      - logging to stdout or stderr
    //      - adding to new_object_files vector
    std::mutex source_files_mtx
    std::mutex ouput_log_mtx, 
    std::mutex object_files_mtx;

    std::vector<fs::path> object_files;
    bool build_success = true;
    
    // Needed for logging messages like this: [2/8] Compiling...
    int count = 1;
    int total = source_files.size();

    std::string error_message = "";

    // additional information used during build/compilation step
    std::vector<std::string> compile_flags = coup_config.get_compile_flags();
    std::string executable_name = coup_config.get_executable();
    std::string cpp_standard = coup_config.get_cpp_version();
    std::string compiler = coup_config.get_compiler();

    auto build_worker = [&] {
        while (1) {
            fs::path source_file;
            {
                std::lock_guard<std::mutex> lock(source_files_mtx);
                if (source_files.empty())
                    return;
                source_file = std::move(source_files.back());
                source_files.pop_back();
            }
            std::string source_filename = get_filename(source_file.string());
            std::string compile_command = 
                make_compile_command(source_file, compiler, 
                                     cpp_standard, compile_flags);
            {
                std::lock_guard<std::mutex> lock(output_log_mtx);
                print_compile(source_filename, compile_command,
                              count++, total, verbose);
            }

            if (!execute_system_call(compile_command.c_str()) {
                {
                    std::lock_guard<std::mutex> lock(output_log_mtx);
                    std::string error = "Failed to compile " + source_filename;
                    print_error(error);
                    error_message += "\n\t" + error;
                }
                build_success = false;
            } else {
                fs::path object_file = 
                    build_directory / replace_extension(source_filename, "o");
                {
                    std::lock_guard<std::mutex> lock(object_files_mtx);
                    object_files.push_back(std::move(object_file));
                }
            }
        }
    };
    
    unsigned int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    unsigned int i;
    for (i = 0; i < num_threads; ++i)
        threads.emplace_back(build_worker);
    for (std::thread& th : threads)
        th.join();

    if (!build_success) {
        assert(!error_message.empty());
        return error_message;
    }

    std::string link_command = make_link_command(new_object_files);
    print_link(executable, link_command, verbose);
    
    if (!execute_system_call(link_command.c_str()))
        return "Linktime error";
    else
        return std::nullopt;

}

std::optional<std::string> coup_project::execute_run(bool verbose) noexcept
{
    std::string executable_name = coup_config.get_executable();
    fs::path executable = build_directory / executable_name;

    if (!fs::exists(executable)) {
        std::optional<std::string> build_resuilt = execute_build(verbose);
        if (build_result.has_value())
            return "Failure during build process\n" + *build_result;
    }

    if (!run(executable)) {
        return "Failed to run " + executable_name;
    } else {
        return std::nullopt;
    }
}

std::optional<std::string> coup_project::execute_clean(bool verbose) noexcept
{
    std::vector<fs::path> build_files = find_obj_files(build_directory);
    fs::path executable = build_directory / coup_config.get_executable();
    if (fs::exists(executable))
        build_files.push_back(std::move(executable));
    
    std::mutex object_files_mtx;
    std::mutex output_log_mtx;
    bool clean_success = true;

    int count = 1;
    int total = object_files.size();

    std::string error_message = "";

    auto clean_worker = [&] {
        while (1) {
            fs::path object_file;
            {
                std::lock_guard<std::mutex> lock(object_files_mtx);
                if (object_files.empty())
                    return;
                object_file = std::move(object_files.back());
                object_files.pop_back();
            }
            std::string object_filename = get_filename(object_file.string());
            std::string rm_command = make_syustem_command("rm", object_file);
            {
                std::lock_guard<std::mutex> lock(output_log_mtx);
                print_remove(object_filename, rm_command, count, total, verbose);
            }

            if (!execute_system_call(rm_command.c_str())) {
                std::string error = "Failed to remove " + object_filename;
                {
                    std::lock_guard<std::mutex> lock(output_log_mtx);
                    print_error(error);
                    error_message += error + '\n';
                }
                clean_success = false;
            }
        }
    };

    unsigned int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    unsigned int i;
    for (i = 0; i < num_threads; ++i)
        threads.emplace_back(clean_worker);
    for (std::thread& th : threads)
        th.join();

    if (!clean_success) {
        assert(!error_message.empty());
        return error_message;
    } else {
        return std::nullopt;
    }
}

/*  Calls execution function corresponding to string command argument
 *  If the execution call returns an optional with a value, execution failed
 *  and an error message will be logged
 *  Otherwise, execution succeeded and execution success along with the
 *  execution runtime will be logged to the user
 */
void coup_project::execute_command(const std::string &command,
								   const std::string &option)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::optional<std::string> result;

	if (command == "build")
	{
		result = execute_build(option == "--verbose" ||
                               option == "-v");
	}
	else if (command == "run")
	{
		result = execute_run();
	}
	else if (command == "clean")
	{
		result = execute_clean(option == "--verbose" ||
                               option == "-v");
	}
	else
	{
		throw std::invalid_argument("Invalid Argument '" + command + "'");
	}

	// no error message
	if (!result.has_value())
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto duration =
			std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		print_result_success(command, duration.count() / 1000.0);
	}
	else
	{
		std::string error_message = *result;
		print_result_failure(command, error_message);
	}
}
} // namespace coup
