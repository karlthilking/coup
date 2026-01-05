#pragma once

#include <filesystem>
#include <vector>

#include "../include/coup_file.hxx"

namespace fs = std::filesystem;
namespace coup {

class coup_project {
 private:
  fs::path src_dir;
  fs::path build_dir;
  std::vector<fs::path> src_files;
  std::vector<fs::path> obj_files;

  /*  Privately accessed constructors, only accessed
   *  directly by the make_project function
   */
  coup_project(const fs::path& src, const fs::path& build,
               const std::vector<fs::path>& source_files,
               const std::vector<fs::path>& object_files);
  coup_project(fs::path&& src, fs::path&& build,
               std::vector<fs::path>&& source_files,
               std::vector<fs::path>&& object_files) noexcept;

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
  
  /*  Compile all source files and link into an executable
   *  Verbose parameter indicates verbose log output option
   *  Returns true if build succeeds, false otherwise
   */ 
  bool execute_build(bool verbose) noexcept;

  /*  Compile all source files, link into executable, and run executable
   *  Verbose parameter indicates verbose log output option
   *  Returns true if run procedure succeeds, false otherwise
   */
  bool execute_run(bool verbose) noexcept;

  /*  Removes all build artifacts from project
   *  Verbose parameter indicates verbose log output option
   *  Returns true if clean finishes successfully, false otherwise
   */
  bool execute_clean(bool verbose) noexcept;

  /*  Receives a string and string, and delegates work to the 
   *  correct function
   *  Checks result of delegated function to handle properly reporting the 
   *  result of an executed command
   *  Valid command strings are: build, run, clean
   *  Option: enable verbose output if option == verbose otherwise disable
   */
  void execute_command(std::string_view command, std::string_view option) noexcept;
};

}  // namespace coup
