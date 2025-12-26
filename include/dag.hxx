#pragma once

#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace coup
{
  enum class file_type { source, header, object };

  struct node
  {
    file_type f_type;
    fs::path f_path;
    std::vector< node* > children;

    node(file_type ft, const fs::path& fp) noexcept;
    void insert(node* child) noexcept; 
    void insert(file_type ft, const fs::path& fp) noexcept;
    fs::file_time_type last_update() const noexcept;
  };
  
  class dag
  {
  private:
    std::vector< node* > nodes;
  public:
    // construct with vector of nodes
    dag(const std::vector< node* >& S) noexcept;
    
    // all copy/moves are deleted
    // only one instance is needed 
    dag(const dag& G) = delete;
    dag(dag&& G) = delete;
    dag& operator=(const dag& G) = delete;
    dag& operator=(dag&& G) = delete;
    ~dag() noexcept;

    std::vector< node* > topological_sort() const noexcept;
    static dag create_dag();
  };
}
