#pragma once

#include <vector>
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <queue>
#include <optional>

namespace fs = std::filesystem;

namespace coup
{
  class dag
  {
  private:
    std::vector< node* > nodes;
  public:
    dag(std::vector< node* > S) noexcept;
    ~dag() noexcept;
    std::vector< node* > topological_sort() const noexcept;
  };

  struct node
  {
    file_type type;
    fs::path path;
    std::vector< node* > children;

    node(file_type ft, const fs::path& p);
    void insert(node* child) noexcept; 
    void insert(file_type ft, const fs::path& p) noexcept;
    fs::file_time_type last_update() const noexcept;
  };
  
  enum class file_type { src, header, obj };
  
}
