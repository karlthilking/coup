#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace fs = std::filesystem;
namespace coup
{

class coup_project
{
 private:
  fs::path src_dir;
  fs::path build_dir;
  fs::path exec_path;
  std::vector<fs::path> src_files;
  std::vector<fs::path> obj_files;

  /*  Privately accessed constructors, only accessed
   *  directly by the make_project function
   */
  coup_project(const fs::path& src, const fs::path& build, const fs::path& exec,
               const std::vector<fs::path>& source_files,
               const std::vector<fs::path>& object_files);

  coup_project(fs::path&& src, fs::path&& build, fs::path&& exec,
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

  /*  Compile all source files and link into an executable
   *  Verbose parameter indicates verbose log output option
   *  Returns nullopt if the build succeeds, or an error
   *  message if any errors occur during the build process
   */
  std::optional<std::string> execute_build(bool verbose) noexcept;

  /*  Compile all source files, link into executable, and run executable
   *  Verbose parameter indicates verbose log output option
   *  Returns nullopt if the executable runs, or an error
   *  message if running fails
   */
  std::optional<std::string> execute_run() noexcept;

  /*  Removes all build artifacts from project
   *  Verbose parameter indicates verbose log output option
   *  Returns nullopt if the build succeeds, or an error
   *  message if an error occurs during the clean
   */
  std::optional<std::string> execute_clean(bool verbose) noexcept;

  /*  Receives a string and string, and delegates work to the
   *  correct function
   *  Checks result of delegated function to handle properly reporting the
   *  result of an executed command
   *  Valid command strings are: build, run, clean
   *  Option: enable verbose output if option == verbose otherwise disable
   */
  void execute_command(const std::string& command, const std::string& option);
};

}  // namespace coup
