#include "../include/coup_project.hxx"

#include <algorithm>
#include <array>
#include <filesystem>
#include <ranges>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../include/coup_file.hxx"
#include "../include/coup_filesystem.hxx"

namespace fs = std::filesystem;
namespace coup {

coup_project::coup_project(const std::vector<coup_file>& coup_files_,
                           const std::vector<fs::path>& src_files_,
                           const std::vector<fs::path>& obj_files_)
    : coup_files(coup_files_), src_files(src_files_), obj_files(obj_files_) {}

coup_project::coup_project(std::vector<coup_file>&& coup_files_,
                           std::vector<fs::path>&& src_files_,
                           std::vector<fs::path>&& obj_files_) noexcept
    : coup_files(std::move(coup_files_)),
      src_files(std::move(src_files_)),
      obj_files(std::move(obj_files_)) {}

/* Obtains all source, header, and object files, associated by filestem
 * Returns a coup_project instance to the caller with properly initialized
 * coup_file instances
 */
coup_project coup_project::make_project() {
  // throws if no root is found
  fs::path root_dir = get_root_dir();

  // map of filename stem to associated paths (source, header, object)
  std::unordered_map<std::string, std::array<fs::path, 3>> file_groups;

  std::vector<fs::path> src_files;
  std::vector<fs::path> obj_files;

  std::thread src_handler([&] {
    src_files = get_src_files(root_dir);
    for (const fs::path& src : src_files) {
      auto src_stem = get_stem(get_filename(src));
      file_groups[src_stem][0] = src;
    }
  });

  std::thread header_handler([&] {
    auto header_files = get_header_files(root_dir);
    for (const fs::path& header : header_files) {
      auto header_stem = get_stem(get_filename(header));
      file_groups[header_stem][1] = header;
    }
  });

  std::thread obj_handler([&] {
    obj_files = get_obj_files(root_dir);
    for (const fs::path& obj : obj_files) {
      auto obj_stem = get_stem(get_filename(obj));
      file_groups[obj_stem][2] = obj;
    }
  });

  src_handler.join();
  header_handler.join();
  obj_handler.join();

  std::vector<coup_file> coup_files;
  for (const auto& [name, files] : file_groups) {
    coup_files.emplace_back(files[0], files[1], files[2]);
  }
  return coup_project(std::move(coup_files), std::move(src_files),
                      std::move(obj_files));
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

bool coup_project::execute_build(bool verbose) const noexcept {
  std::mutex files_mtx;
  std::mutex log_mtx;
  bool build_success = true; 
  std::atomic<int> log_count{1};
  int log_total = src_files.size();

  if (verbose) {
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
        
        std::string src_name = cur_file.string();
        std::string compile_command = make_compile_command(cur_file);

        {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_compile(src_name, compile_command, log_count.fetch_add(1),
                        log_total, true);
        }
                        
        if (!execute_system_call(compile_command.c_str())) {
          std::lock_guard<std::mutex> lock(log_mtx);
          print_error("Failed to compile" + src_name);
          build_success = false;
        }
      }
    };
    
    unsigned int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (unsigned int i{}; i < num_threads; ++i) {
      threads.emplace_back(build_worker);
    }
    for (std::thread& th : threads) {
      th.join();
    }
    return build_success;
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

        std::string src_name = cur_file.string();
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

    unsigned int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (unsigned int i{}; i < num_threads; ++i) {
      threads.emplace_back(build_worker);
    }
    for (std::thread& th : threads) {
      th.join();
    }
    return build_success;
  }
}

bool coup_project::execute_run(bool verbose) const noexcept {
  if (!execute_build(verbose)) {
    
    return false;
  }
  
}

bool coup_project::execute_clean(bool verbose) const noexcept {

}

void coup_project::execute_command(std::string_view command,
                                   std::string_view option) const noexcept {
  
}
}  // namespace coup
