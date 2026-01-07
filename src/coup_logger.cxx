<<<<<<< HEAD
#include <stdexcept>
#include <string_view>
#include <filesystem>
#include <iostream>
#include <cassert>
#include "../include/coup_logger.hxx"
=======
#include "../include/coup_logger.hxx"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

>>>>>>> refs/remotes/origin/main
#include "../include/coup_filesystem.hxx"
#include "../include/coup_system.hxx"

namespace fs = std::filesystem;
namespace coup {
<<<<<<< HEAD

coup_logger(int total, bool verbose)
  : log_count(0),
    log_total(total)
    verbose_output(verbose)
{}

=======
>>>>>>> refs/remotes/origin/main
/*  Print message explanation user to the user if:
 *    - user provides no arguments
 *    - user provides an invalid argument
 */
<<<<<<< HEAD
void coup_logger::print_usage() const noexcept {
  std::cerr << "Usage: ./coup <command> <option>\nCommands:\n"
    << "  build: Compile and link source files into executable\n"
    << "  run: Complete build step and run executable\n"
    << "  clean: Remove build artifacts\n";
    << "Options:\n  verbose: Enable verbose ouput during command execution\n";
}

void coup_logger::print_error(const std::string& error_message) const noexcept {
=======
void print_usage()
{
  std::cerr << "Usage: ./coup <command> <option>\nCommands:\n"
            << "  build: Compile and link source files into executable\n"
            << "  run: Complete build step and run executable\n"
            << "  clean: Remove build artifacts\n"
  << "Options:\n  verbose: Enable verbose ouput during execution\n";
}

// Error logging for generally occuring errors
void print_error(const std::string& error_message)
{
>>>>>>> refs/remotes/origin/main
  std::cerr << "Error: " << error_message << "\n";
}

/*  Print log message indicating a file compilation occuring
 *  If verbose output is enabled, provide compilation count and total,
 *  and display compile command
 */
<<<<<<< HEAD
void coup_logger::print_compile(std::string_view src_name,
                                std::string_view compile_command) noexcept {
  if (verbose_output) {
    std::cout << "[" << log_count++ << "/" << log_total 
      << "] Compiling " << src_name << "\n";
    std::cout << "  $ " << compile_command << "\n";
  } else {
=======
void print_compile(std::string_view src_name,
                   std::string_view compile_command,
                   int log_count, int log_total,
                   bool verbose_output)
{
  if (verbose_output)
  {
    std::cout << "[" << log_count++ << "/" << log_total << "] Compiling "
              << src_name << "\n";
    std::cout << "  $ " << compile_command << "\n";
  }
  else
  {
>>>>>>> refs/remotes/origin/main
    std::cout << "Compiling " << src_name << "\n";
  }
}

/*  Print log message indicating a linkage step occuring
 *  If verbose output is enabled, provide link command used
 */
<<<<<<< HEAD
void coup_logger::print_link(std::string_view exec_name,
                             std::string_view link_command) const noexcept {
  if (verbose_output) {
    std::cout << "Linking " << exec_name << "\n  $ "
      << link_command << "\n";
  } else {
=======
void print_link(const std::string& exec_name,
                std::string_view link_command,
                bool verbose_output)
{
  if (verbose_output)
  {
    std::cout << "Linking " << exec_name << "\n  $ " << link_command << "\n";
  }
  else
  {
>>>>>>> refs/remotes/origin/main
    std::cout << "Linking " << exec_name << "\n";
  }
}

/*  Print log message indicating a file removal during a clean
 *  If verbose output is enabled, provide current removed out of total
 *  file removals, and provide remove command used to delete file
 */
<<<<<<< HEAD
void coup_logger::print_remove(std::string_view file_name,
                               std::string_view rm_command) noexcept {
  if (verbose_output) {
    std::cout << "[" << log_count++ << "/" << total_count << "] Removing "
      << file_name << "\n";
    std::cout << "  $ " << rm_command << "\n";
  } else {
=======
void print_remove(std::string_view file_name,
                  std::string_view rm_command,
                  int log_count, int log_total, 
                  bool verbose_output)
{
  if (verbose_output)
  {
    std::cout << "[" << log_count++ << "/" << log_total << "] Removing "
              << file_name << "\n";
    std::cout << "  $ " << rm_command << "\n";
  }
  else
  {
>>>>>>> refs/remotes/origin/main
    std::cout << "Removing " << file_name << "\n";
  }
}

/*  Determine which command executed successfully and delegate
 *  logging to the matching function
 */
<<<<<<< HEAD
void coup_logger::print_result_success(std::string_view command,
                                       double runtime) const noexcept {
  if (command == "build") {
    print_build_success(runtime);
  } else if (command == "run") {
    print_run_success(runtime);
  } else if (command == "clean") {
    print_clean_success(runtime);
  } else {
=======
void print_result_success(std::string_view command, double runtime)
{
  if (command == "build")
  {
    print_build_success(runtime);
  }
  else if (command == "run")
  {
    print_run_success(runtime);
  }
  else if (command == "clean")
  {
    print_clean_success(runtime);
  }
  else
  {
>>>>>>> refs/remotes/origin/main
    // should never reach this branch
    assert(false && "print_result_success received invalid command");
  }
}

/*  Determine which command failed and delegate logging to matching function
 */
<<<<<<< HEAD
void coup_logger::print_result_failure(std::string_view command,
                                       const std::string& error_message) const noexcept {
  if (command == "build") {
    print_build_failure(error_message);
  } else if (command == "run") {
    print_run_failure(error_message);
  } else if (command == "clean") {
    print_clean_failure(error_message);
  } else {
=======
void print_result_failure(std::string_view command,
                          const std::string& error_message)
{
  if (command == "build")
  {
    print_build_failure(error_message);
  }
  else if (command == "run")
  {
    print_run_failure(error_message);
  }
  else if (command == "clean")
  {
    print_clean_failure(error_message);
  }
  else
  {
>>>>>>> refs/remotes/origin/main
    // should never reach this branch
    assert(false && "print_result_failure received invalid command");
  }
}

// log build success
<<<<<<< HEAD
void coup_logger::print_build_success(double runtime) const noexcept {
=======
void print_build_success(double runtime)
{
>>>>>>> refs/remotes/origin/main
  std::cout << "Build succeeded in " << runtime << "s\n";
}

// log build failure
<<<<<<< HEAD
void coup_logger::print_build_failure(const std::string& error_message) const noexcept {
=======
void print_build_failure(const std::string& error_message)
{
>>>>>>> refs/remotes/origin/main
  std::cout << "Build failed: " << error_message << "\n";
}

// log run success
<<<<<<< HEAD
void coup_logger::print_run_success(double runtime) const noexcept {
=======
void print_run_success(double runtime)
{
>>>>>>> refs/remotes/origin/main
  std::cout << "Run succeeded in " << runtime << "s\n";
}

// log run failure
<<<<<<< HEAD
void coup_logger::print_run_failure(const std::string& error_message) const noexcept {
=======
void print_run_failure(const std::string& error_message)
{
>>>>>>> refs/remotes/origin/main
  std::cout << "Run failed: " << error_message << "\n";
}

// log clean success
<<<<<<< HEAD
void coup_logger::print_clean_success(double runtime) const noexcept {
=======
void print_clean_success(double runtime)
{
>>>>>>> refs/remotes/origin/main
  std::cout << "Clean succeeded in " << runtime << "s\n";
}

// log clean failure
<<<<<<< HEAD
void coup_logger::print_clean_failure(const std::string& error_message) const noexcept {
  std::cout << "Clean failed: " << error_message << "\n";
}
} // namespace coup
=======
void print_clean_failure(const std::string& error_message)
{
  std::cout << "Clean failed: " << error_message << "\n";
}
}  // namespace coup
>>>>>>> refs/remotes/origin/main
