#pragma once

#include <filesystem>
#include <vector>
#include "../include/coup_file.hxx"

namespace fs = std::filesystem;
namespace coup {
  
class coup_project {
private:
  // manages all coup_files 
  std::vector<coup_file> coup_files;
  
  // constructors are private and only to be accessed by make_project() function 
  coup_project(const std::vector<coup_file>& files);
  coup_project(std::vector<coup_file>&& files) noexcept;
public:
  // static creational function
  static coup_project make_project();
  // returns const reference to vector of project source files
  std::vector<fs::path> get_project_src_files() const noexcept;
};

} // namespace coup
