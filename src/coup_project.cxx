#include <algorithm>
#include <array>
#include <filesystem>
#include <ranges>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../include/coup_logger.hxx"
#include "../include/coup_project.hxx"
#include "../include/coup_file.hxx"
#include "../include/coup_filesystem.hxx"

namespace fs = std::filesystem;
namespace coup {

coup_project::coup_project(const fs::path& src, const fs::path& build,
                           const std::vector<fs::path>& source_files,
                           const std::vector<fs::path>& object_files) 
  : src_dir(src),
    build_dir(build),
    src_files(source_files),
    obj_files(object_files)
{}

coup_project::coup_project(fs::path&& src, fs::path&& build, 
                           std::vector<fs::path>&& source_files,
                           std::vector<fs::path>&& object_files) noexcept
  : src_dir(std::move(src)),
    build_dir(std::move(build)),
    src_files(std::move(source_files)),
    obj_files(std::move(object_files))
{}

/* Obtains all source, header, and object files, associated by filestem
 * Returns a coup_project instance to the caller with properly initialized
 * coup_file instances
 */
coup_project coup_project::make_project() {
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
std::vector<fs::path> coup_project::get_project_src_files() const noexcept {
  return src_files;
}

// return obj files
std::vector<fs::path> coup_project::get_project_obj_files() const noexcept {
  return obj_files;
}

int coup_project::num_src_files() const noexcept { return src_files.size(); }

int coup_project::num_obj_files() const noexcept { return obj_files.size(); }

/*  Set/update dependency information for each coup file
 *
 *  For every coup_file that:
 *    - Has no generated dependency file or
 *    - Has an outdated dependency file
 *  - Retrieve up to date dependency info
 *  - Set its dependency file
 *  - Store dependencies in each coup_file class
 */
void coup_project::set_dependencies() noexcept {
  ;
  /*
  std::ranges::for_each(coup_files, [this](const coup_file& cf) {
    if (cf.requires_dep_update()) {
      auto deps = get_dependencies(cf.get_src());
      cf.set_dep(make_dep_file(cf.get_src()));
      for (const auto& dep : deps) {
        cf.add_dependency(&coup_file_map[dep]);
      }
    }
  });
  */
}

bool coup_project::execute_build(bool verbose) noexcept {
  std::mutex src_files_mtx;
  std::mutex log_mtx;
  bool build_success = true; 

  unsigned int num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  threads.reserve(num_threads);
  
  std::mutex obj_files_mtx;
  std::vector<fs::path> new_obj_files;

  if (verbose) {
    std::atomic<int> log_count{1};
    int log_total = src_files.size();

    auto build_worker = [&]() {
      for (;;) {
        fs::path cur_file;
        {
          std::lock_guard<std::mutex> lock(src_files_mtx);
          if (src_files.empty()) {
            return;
          }
          cur_file = std::move(src_files.back());
          src_files.pop_back();
        }
        
        std::string src_name = get_filename(cur_file.string());
        std::string compile_command = make_compile_command(cur_file);

        {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_compile(src_name, compile_command, log_count.fetch_add(1),
                        log_total, true);
        }
                        
        if (!execute_system_call(compile_command.c_str())) {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_error("Failed to compile: " + src_name);
          build_success = false;
        } else {
          fs::path obj_file = build_dir / replace_extension(src_name, "o");
          {
            std::lock_guard<std::mutex> lock(obj_files_mtx);
            new_obj_files.push_back(obj_file);
          }
        }
      }
    };
    
    for (unsigned int i{}; i < num_threads; ++i) {
      threads.emplace_back(build_worker);
    }
    for (std::thread& th : threads) {
      th.join();
    }
    
    if (!build_success || src_files.size() != new_obj_files.size()) {
      return false;
    }
    
    std::string link_command = make_link_command(new_obj_files);
    {
      std::lock_guard<std::mutex> lock(log_mtx);
      print_link(link_command, true);
    }
    
    if (!execute_system_call(link_command.c_str())) {
      std::lock_guard<std::mutex> lock(log_mtx);
      print_error("Failed to link: coup_exec");
      return false;
    } else {
      return true;
    }
  } else {

    auto build_worker = [&]() {
      for (;;) {
        fs::path cur_file;
        {
          std::lock_guard<std::mutex> lock(files_mtx);
          if (src_files.empty()) {
            return;
          }
          cur_file = std::move(src_files.back());
          src_files.pop_back();
        }

        std::string src_name = get_filename(cur_file.string());
        std::string compile_command = make_compile_command(cur_file);

        {
          std::lock_guard<std::mutex> lock(cout_mtx);
          print_compile(src_name, compile_command, 0, 0, false);
        }

        if (!execute_system_call(compile_command.c_str())) {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_error("Failed to compile " + src_name);
          build_success = false;
        }
      }
    };
    
    for (unsigned int i{}; i < num_threads; ++i) {
      threads.emplace_back(build_worker);
    }
    for (std::thread& th : threads) {
      th.join();
    }
    return build_success;
  }
}

bool coup_project::execute_run(bool verbose) noexcept {
  // TODO: executable may require full path if
  // created in build directory
  if (!fs::exists("coup_exec")) {
    return false;
  } else {
    return run(fs::path("coup_exec"));
  }
}

bool coup_project::execute_clean(bool verbose) noexcept {
  std::mutex file_mtx;
  std::mutex log_mtx; 
  bool clean_success = true;
  unsigned int num_threads = std::thread::hardware_concurrency();
  std::vector<std::threads> threads;
  threads.reserve(num_threads);

  if (verbose) {
    std::atomic<int> log_count{1};
    int log_total = obj_files.size();

    auto clean_worker = [&]() {
      for (;;) {
        fs::path cur_file;
        {
          std::lock_guard<std::mutex> lock(file_mtx);
          if (obj_files.empty()) {
            return;
          }
          cur_file = obj_files.back();
          obj_files.pop_back();
        }
        
        std::string obj_name = get_filename(cur_file.string());
        std::string rm_command = make_system_command("rm", cur_file);
        
        {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_remove(obj_name, rm_command, log_count.fetch_add(1), log_total, true);
        }
        
        if (!execute_system_call(rm_command.c_str())) {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_error("Failed to remove " + obj_name);
          clean_success = false;
        }
      }
    }

    for (unsigned int i{}; i < num_threads; ++i) {
      threads.emplace_back(worker);
    }
    for (std::thread& th: threads) {
      th.join();
    }
    return clean_success;
  } else {
    auto clean_worker = [&]() {
      for (;;) {
        fs::path cur_file;
        {
          std::lock_guard<std::mutex> lock(file_mtx);
          if (obj_files.empty()) {
            return;
          }
          cur_file = obj_files.back();
          obj_files.pop_back();
        }

        std::string obj_name = get_filename(cur_file.string());
        std::string rm_command = make_system_command("rm", cur_file);

        {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_remove(obj_name, rm_command, 0, 0, false);
        }

        if (!execute_system_call(rm_command)) {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_error("Failed to clean: " + obj_name);
          clean_success = false;
        }
      }
    };
    
    for (unsigned int i{}; i < num_threads; ++i) {
      threads.emplace_back(clean_worker);
    }
    for (std::thread& th : threads) {
      th.join();
    }
    return clean_success;
  }
}

void coup_project::execute_command(std::string_view command,
                                   std::string_view option) const noexcept {
  
}
}  // namespace coup
