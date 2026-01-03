#include <array>
#include <filesystem>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>
#include <ranges>
#include "../include/coup_project.hxx"
#include "../include/coup_file.hxx"
#include "../include/coup_filesystem.hxx"

namespace fs = std::filesystem;
namespace coup {

coup_project::coup_project(const std::vector<coup_file>& files)
    : coup_files(files) {}

coup_project::coup_project(std::vector<coup_file>&& files) noexcept
    : coup_files(std::move(files)) {}

/* Obtains all source, header, and object files, associated by filestem
 * Returns a coup_project instance to the caller with properly initialized
 * coup_file instances
 */
coup_project coup_project::make_project() {
  // throws if no root is found
  fs::path root_dir = get_root_dir();

  // map of filename stem to associated paths (source, header, object)
  std::unordered_map<std::string, std::array<fs::path, 3>> file_groups;

  std::thread src_handler([&] {
    auto src_files = get_src_files(root_dir);
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
    auto obj_files = get_obj_files(root_dir);
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
  return coup_project(std::move(coup_files));
}

// Returns a vector of all project source files
std::vector<fs::path> coup_project::get_project_src_files() const noexcept {
  std::vector<fs::path> src_files;
  src_files.reserve(coup_files.size());
  
  #if(__cpp_lib_ranges >= 201911L)
    std::ranges::for_each(coup_files, [&](const coup_file& cf) {
      if (cf.has_src()) {
        src_files.push_back(cf.get_src());
      }
    });
  #else
    std::for_each(begin(coup_files), end(coup_files),
      [&](const coup_file& cf) {
        if (cf.has_src()) {
          src_files.push_back(cf.get_src());
        }
      }
    );
  #endif
  return src_files;
}

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
}  // namespace coup
