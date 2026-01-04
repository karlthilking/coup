#include <stdexcept>
#include <string_view>
#include <filesystem>
#include <iostream>
#include <cassert>
#include "../include/coup_logger.hxx"
#include "../include/coup_filesystem.hxx"
#include "../include/coup_system.hxx"

namespace fs = std::filesystem;
namespace coup {

coup_logger(int total, bool verbose)
  : log_count(0),
    log_total(total)
    verbose_output(verbose)
{}

/*  Print message explanation user to the user if:
 *    - user provides no arguments
 *    - user provides an invalid argument
 */
void coup_logger::print_usage() const noexcept {
  std::cerr << "Usage: ./coup <command> <option>\nCommands:\n"
    << "  build: Compile and link source files into executable\n"
    << "  run: Complete build step and run executable\n"
    << "  clean: Remove build artifacts\n";
    << "Options:\n  verbose: Enable verbose ouput during command execution\n";
}

void coup_logger::print_error(const std::string& error_msg) const noexcept {
  std::cerr << "Error: " << error_msg << "\n";
}

/*  Print log message indicating a file compilation occuring
 *  If verbose output is enabled, provide compilation count and total,
 *  and display compile command
 */
void coup_logger::print_compile(std::string_view src_name,
                                std::string_view compile_command) noexcept {
  if (verbose_output) {
    std::cout << "[" << log_count++ << "/" << log_total 
      << "] Compiling " << src_name << "\n";
    std::cout << "  $ " << compile_command << "\n";
  } else {
    std::cout << "Compiling " << src_name << "\n";
  }
}

/*  Print log message indicating a linkage step occuring
 *  If verbose output is enabled, provide link command used
 */
void coup_logger::print_link(std::string_view exec_name,
                             std::string_view link_command) const noexcept {
  if (verbose_output) {
    std::cout << "Linking " << exec_name << "\n  $ "
      << link_command << "\n";
  } else {
    std::cout << "Linking " << exec_name << "\n";
  }
}

/*  Print log message indicating a file removal during a clean
 *  If verbose output is enabled, provide current removed out of total
 *  file removals, and provide remove command used to delete file
 */
void coup_logger::print_remove(std::string_view file_name,
                               std::string_view rm_command) noexcept {
  if (verbose_output) {
    std::cout << "[" << log_count++ << "/" << total_count << "] Removing "
      << file_name << "\n";
    std::cout << "  $ " << rm_command << "\n";
  } else {
    std::cout << "Removing " << file_name << "\n";
  }
}

/*  Determine which command executed successfully and delegate
 *  logging to the matching function
 */
void coup_logger::print_result_success(std::string_view command,
                                       double runtime) const noexcept {
  if (command == "build") {
    print_build_success(runtime);
  } else if (command == "run") {
    print_run_success(runtime);
  } else if (command == "clean") {
    print_clean_success(runtime);
  } else {
    // should never reach this branch
    assert(false && "print_result_success received invalid command");
  }
}

/*  Determine which command failed and delegate logging to matching function
 */
void coup_logger::print_result_failure(std::string_view command,
                                       const std::exception& e) const noexcept {
  if (command == "build") {
    print_build_failure(e);
  } else if (command == "run") {
    print_run_failure(e);
  } else if (command == "clean") {
    print_clean_failure(e);
  } else {
    // should never reach this branch
    assert(false && "print_result_failure received invalid command");
  }
}

// log build success
void coup_logger::print_build_success(double runtime) const noexcept {
  std::cout << "Build succeeded in " << runtime << "s\n";
}

// log build failure
void coup_logger::print_build_failure(const std::exception& e) const noexcept {
  std::cout << "Build failed: " << e.what() << "\n";
}

// log run success
void coup_logger::print_run_success(double runtime) const noexcept {
  std::cout << "Run succeeded in " << runtime << "s\n";
}

// log run failure
void coup_logger::print_run_failure(const std::exception& e) const noexcept {
  std::cout << "Run failed: " << e.what() << "\n";
}

// log clean success
void coup_logger::print_clean_success(double runtime) const noexcept {
  std::cout << "Clean succeeded in " << runtime << "s\n";
}

// log clean failure
void coup_logger::print_clean_failure(const std::exception& e) const noexcept {
  std::cout << "Clean failed: " << e.what() << "\n";
}
} // namespace coup
