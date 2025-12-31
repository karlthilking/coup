#pragma once

#include <vector>

#include "../include/coup_file.hxx"  // coup_file

namespace coup {
class coup_project {
 private:
  std::vector<coup_file> coup_files;

  coup_project(const std::vector<coup_file>& coup_files);
  coup_project(std::vector<coup_file>&& coup_files) noexcept;

 public:
  static coup_project create_project();

  [[nodiscard]] const std::vector<coup_file>& get_files() const noexcept;
};
}  // namespace coup
