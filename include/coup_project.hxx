#pragma once

#include <filesystem>
#include <vector>

#include "../include/coup_file.hxx"

namespace fs = std::filesystem;
namespace coup {

class coup_project {
 private:
  std::vector<coup_file> coup_files;
  std::vector<fs::path> src_files;
  std::vector<fs::path> obj_files;

  /*  Privately accessed constructors, only accessed
   *  directly by the make_project function
   */
  coup_project(const std::vector<coup_file>& coup_files_,
               const std::vector<fs::path>& src_files_,
               const std::vector<fs::path>& obj_files_);
  coup_project(std::vector<coup_file>&& coup_files_,
               std::vector<fs::path>&& src_files_,
               std::vector<fs::path>&& obj_files_) noexcept;

 public:
  // Static function that returns a coup_project
  static coup_project make_project();
  
  /*  Obtains source files if they have not been
   *  instantiated and returns project source files
   *  to caller
   *  Source files are found lazily, they are not set
   *  during construction of a coup_project
   */ 
  std::vector<fs::path> get_project_src_files() const noexcept;
  
  // Obtains all project object and executable files
  std::vector<fs::path> get_project_obj_files() const noexcept;
  
  // return number of project source files
  int num_src_files() const noexcept;

  // return number of project object files
  int num_obj_files() const noexcept;

  // Sets dependencies for each coup_file
  void set_dependencies() noexcept;
};

}  // namespace coup
