#pragma once

#include <string>
#include <string_view>

namespace coup
{
void print_usage();

void print_error(const std::string& error_message);

void print_compile(std::string_view src_name, std::string_view compile_command,
                   int log_count, int log_total, bool verbose_output);

void print_link(const std::string& exec_name, std::string_view link_command,
                bool verbose_output);

void print_remove(std::string_view file_name, std::string_view rm_command,
                  int log_count, int log_total, bool verbose_output);

void print_result_success(std::string_view command, double runtime);

void print_result_failure(std::string_view command,
                          const std::string& error_message);

void print_build_success(double runtime);

void print_build_failure(const std::string& error_message);

void print_run_success(double runtime);

void print_run_failure(const std::string& error_message);

void print_clean_success(double runtime);

void print_clean_failure(const std::string& error_message);

}  // namespace coup
