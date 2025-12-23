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
    std::vector< node* > src_nodes;
    std::vector< node* > sink_nodes;
  public:
    dag(std::vector< node* > s, std::vector< node* > t) noexcept;
    std::vector< fs::path > topological_sort() const noexcept;
    
  };

  struct node
  {
    file_type type;
    fs::path path;
    std::vector< node* > children;

    node(file_type ft, const fs::path& pth)
      : type(ft), path(pth)
    {
      assert(fs::exists(path));
    }

    void insert(node* child) noexcept { children.push_back(child); }
    
    fs::file_time_type get_last_update() const noexcept
    {
      return fs::last_write_time(path);
    }
  };
  
  enum class file_type { source, header };
  
}
