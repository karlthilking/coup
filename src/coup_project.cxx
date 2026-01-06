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
namespace coup {

coup_project::coup_project(const fs::path& src, const fs::path& build,
                           const std::vector<fs::path>& source_files,
                           const std::vector<fs::path>& object_files)
    : src_dir(src),
      build_dir(build),
      src_files(source_files),
      obj_files(object_files)
{
}

coup_project::coup_project(fs::path&& src, fs::path&& build,
                           std::vector<fs::path>&& source_files,
                           std::vector<fs::path>&& object_files) noexcept
    : src_dir(std::move(src)),
      build_dir(std::move(build)),
      src_files(std::move(source_files)),
      obj_files(std::move(object_files))
{
}

/* Obtains all source, header, and object files, associated by filestem
 * Returns a coup_project instance to the caller with properly initialized
 * coup_file instances
 */
coup_project coup_project::make_project()
{
  // throws if no root is found
  fs::path root_dir = get_root_dir();
  fs::path src_dir = get_src_dir(root_dir);
  fs::path build_dir = get_out_dir(root_dir);

  std::vector<fs::path> src_files = find_src_files(src_dir);
  std::vector<fs::path> obj_files = find_obj_files(build_dir);

  return coup_project(std::move(src_dir), std::move(build_dir),
                      std::move(src_files), std::move(obj_files));
}

// return source files
std::vector<fs::path> coup_project::get_project_src_files() const noexcept
{
  return src_files;
}

// return obj files
std::vector<fs::path> coup_project::get_project_obj_files() const noexcept
{
  return obj_files;
}

int coup_project::num_src_files() const noexcept
{
  return src_files.size();
}

int coup_project::num_obj_files() const noexcept
{
  return obj_files.size();
}


std::optional<std::string> coup_project::execute_build(bool verbose) noexcept
{
  /*  Locking critical sections:
   *    - src_files: worker threads pop from src_files vector (during
   * compilation steps)
   *    - logging: protecting simultaneous stdout or stderr logging
   *    - obj_files: worker threads push to obj_files vector (for linking step)
   */
  std::mutex src_files_mtx;
  std::mutex log_mtx;
  std::mutex obj_files_mtx;

  std::vector<fs::path> new_obj_files;
  bool build_success = true;

  std::atomic<int> log_count{1};
  int log_total = src_files.size();

  std::string error_message = "";

  auto build_worker = [&]() {
    for (;;)
    {
      fs::path cur_src;
      {
        std::lock_guard<std::mutex> lock(src_files_mtx);
        if (src_files.empty())
        {
          return;
        }
        cur_src = std::move(src_files.back());
        src_files.pop_back();
      }

      std::string src_name = get_filename(cur_src.string());
      std::string compile_command = make_compile_command(cur_src);

      {
        std::lock_guard<std::mutex> lock(log_mtx);
        print_compile(src_name, compile_command, log_count.fetch_add(1),
                      log_total, verbose);
      }

      if (!execute_system_call(compile_command.c_str()))
      {
        {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_error("Failed to compile " + src_name);
          error_message += "\n  Failed to compile " + src_name;
        }
        build_success = false;
      }
      else
      {
        fs::path obj_file = build_dir / replace_extension(src_name, "o");
        {
          std::lock_guard<std::mutex> lock(obj_files_mtx);
          new_obj_files.push_back(obj_file);
        }
      }
    }
  };

  std::vector<std::thread> threads;
  unsigned int num_threads = std::thread::hardware_concurrency();
  threads.reserve(num_threads);

  for (unsigned int i{}; i < num_threads; ++i)
  {
    threads.emplace_back(build_worker);
  }
  for (std::thread& th : threads)
  {
    th.join();
  }

  if (!build_success || new_obj_files.size() < src_files.size())
  {
    assert(!error_message.empty());
    return error_message;
  }

  std::string link_command = make_link_command(new_obj_files);
  print_link("coup_exec", link_command, verbose);

  if (!execute_system_call(link_command.c_str()))
  {
    return "Failed to link object files to create coup_exec";
  }
  else
  {
    return std::nullopt;
  }
}

std::optional<std::string> coup_project::execute_run(bool verbose) noexcept
{
  fs::path exec_file = build_dir / "coup_exec";
  if (!fs::exists(exec_file))
  {
    return "No executable found, build your project to create one";
  }
  else if (!run(exec_file))
  {
    return "Failed to run coup_exec";
  }
  else
  {
    return std::nullopt;
  }
}

std::optional<std::string> coup_project::execute_clean(bool verbose) noexcept
{
  std::mutex obj_files_mtx;
  std::mutex log_mtx;
  bool clean_success = true;

  std::atomic<int> log_count{1};
  int log_total = obj_files.size();

  std::string error_message = "";

  auto clean_worker = [&]() {
    for (;;)
    {
      fs::path cur_file;
      {
        std::lock_guard<std::mutex> lock(obj_files_mtx);
        if (obj_files.empty())
        {
          return;
        }
        cur_file = obj_files.back();
        obj_files.pop_back();
      }

      std::string obj_name = get_filename(cur_file.string());
      std::string rm_command = make_system_command("rm", cur_file);

      {
        std::lock_guard<std::mutex> lock(log_mtx);
        print_remove(obj_name, rm_command,
                     log_count.fetch_add(1),
                     log_total, verbose);
      }

      if (!execute_system_call(rm_command.c_str()))
      {
        {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_error("Failed to remove " + obj_name);
          error_message += "\nFailed to remove " + obj_name;
        }
        clean_success = false;
      }
    }
  };

  unsigned int num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  for (unsigned int i{}; i < num_threads; ++i)
  {
    threads.emplace_back(clean_worker);
  }

  for (std::thread& th : threads)
  {
    th.join();
  }

  if (!clean_success || obj_files.size() > 0)
  {
    assert(!error_message.empty());
    return error_message;
  }
  else
  {
    return std::nullopt;
  }
}

/*  Calls execution function corresponding to string command argument
 *  If the execution call returns an optional with a value, execution failed
 *  and an error message will be logged
 *  Otherwise, execution succeeded and execution success along with the
 *  execution runtime will be logged to the user
 */
void coup_project::execute_command(const std::string& command,
                                   const std::string& option)
{
  auto start = std::chrono::high_resolution_clock::now();
  std::optional<std::string> result;

  if (command == "build")
  {
    result = execute_build(option == "verbose");
  }
  else if (command == "run")
  {
    result = execute_run(option == "verbose");
  }
  else if (command == "clean")
  {
    result = execute_clean(option == "verbose");
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
}  // namespace coup
