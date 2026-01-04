#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;
namespace coup {

class coup_logger {
private:
  int log_count;
  int log_total;
  bool verbose_output;
public:
  coup_logger(int total, bool verbose);

  static void print_usage() const noexcept;

  void print_compile(std::string_view src_name,
                     std::string_view compile_command = "") noexcept;
  void print_link(std::string_view exec_name,
                  std::string_view link_command = "") const noexcept;
  void print_remove(std::string_view file_name,
                    std::string_view rm_command = "") noexcept;

  void print_result_success(std::string_view command, double runtime) const noexcept;
  void print_result_failure(std::string_view command,
                            const std::exception& e) const noexcept;

  void print_build_success(double runtime) const noexcept;
  void print_build_failure(const std::exception& e) const noexcept;

  void print_run_success(double runtime) const noexcept;
  void print_run_failure(const std::exception& e) const noexcept;

  void print_clean_success(double runtime) const noexcept;
  void print_clean_failure(const std::exception& e) const noexcept;
};
} // namespace coup
