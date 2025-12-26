#include <vector>
#include <filesystem> // path, file_time_type, last_write_time
#include <unordered_map>
#include <queue>
#include <cassert>
#include <utility>
#include <optional>
#include <stdexcept> // runtime_error

#include "../include/dag.hxx"
#include "../include/file_tracker.hxx" // get_root, get_src_dir, get_include_dir

namespace fs = std::filesystem;

namespace coup
{
  // ---------------------- NODE ----------------------
  node::node(file_type ft, const fs::path& fp) noexcept
   : f_type(ft), 
     f_path(fp)
  {}

  void node::insert(node* v) noexcept
  {
   children.push_back(v);
  }

  void node::insert(file_type ft, const fs::path& fp) noexcept
  {
   children.emplace_back(new node(ft, fp));
  }

  fs::file_time_type node::last_update() const noexcept
  {
   return fs::last_write_time(f_path);
  }
  
  // -------------------- DAG ----------------------
  dag::dag(const std::vector< node* >& S) noexcept
   : nodes(S)
  {}

  dag::~dag() noexcept
  {
    for(node* u: nodes)
    {
      assert(u != nullptr);
      delete(u);
    }
  }

  std::vector< node* > dag::topological_sort() const noexcept
  {
    // topologically sort project files based on 
    // dependencies to inform compilation order
    
    std::unordered_map< node*, int > indegree;
    for(const node* u: nodes)
    {
      for(node* v: u->children)
      {
        ++indegree[v];
      }
    }

    std::queue< node* > q;
    for(node* u: nodes)
    {
      if(indegree[u] == 0)
      {
        q.push(u);
      }
    }
    assert(!q.empty());

    std::vector< node* > topological_order;
    while(!q.empty())
    {
      node* u = q.front(); q.pop();
      topological_order.push_back(u);

      for(node* v: u->children)
      {
        int deps = --indegree[v];
        if(deps == 0)
        {
          q.push(v);
        }
      }
    }
    assert(topological_order.size() == nodes.size());
    return topological_order;
  }
  
  static std::vector< node* > 
  dag::merge_node_vecs(std::vector< node* >&& src_nodes,
                       std::vector< node* >&& header_nodes,
                       std::vector< node* >&& obj_nodes)
  {
    src_nodes.reserve(src_nodes.size() +
                      header_nodes.size() + 
                      obj_nodes.size());

    src_nodes.insert(end(src_nodes),
                     begin(header_nodes),
                     end(header_nodes));

    src_nodes.insert(end(src_nodes),
                     begin(obj_nodes),
                     end(obj_nodes));
    
    return src_nodes;
  }

  static std::vector< node* >
  dag::create_node_vec(std::vector< fs::path >&& src_files,
                       std::vector< fs::path >&& header_files,
                       std::vector< fs::path >&& obj_files)
  {
    std::vector< node* > src_nodes;
    std::vector< node* > header_nodes;
    std::vector< node* > obj_nodes;

    for(const fs::path& p: src_files)
    {
      src_nodes.emplace_back(file_type::source, p);
    }
    for(const fs::path& p: header_files)
    {
      header_nodes.emplace_back(file_type::header, p);
    }
    for(const fs::path& p: obj_files)
    {
      obj_nodes.emplace_back(file_type::object, p);
    }
    
    for(node* u: src_nodes)
    {
      // TODO: implement dependency tracking
      u->children = get_dependencies(u->f_path);
    }
    
    std::vector< node* > nodes = merge_node_vecs(std::move(src_nodes),
                                                 std::move(header_nodes),
                                                 std::move(obj_nodes));
    return nodes;
  }
  
  static dag dag::create_dag()
  {
    // create a DAG instance using file tracker utilities
    // to find all relevant project files
    
    std::optional< fs::path > r_opt = get_root();
    if(!r_opt.has_value())
    {
      throw std::runtime_error("[ERROR] No root directory found. Define a src or include directory to register a root directory.\n");
    }

    fs::path root = r_opt.value();
    std::optional< fs::path > src_opt = get_src_dir(root);
    std::optional< fs::path > inc_opt = get_include_dir(root);
    
    std::vector< fs::path > obj_files = get_obj_files(root);
    std::vector< fs::path > src_files;
    std::vector< fs::path > header_files;

    if(!src_opt.has_value())
    {
      std::cout << "[WARNING] No src directory found.\n";
    }
    else
    {
      fs::path src_dir = src_opt.value();
      src_files = get_src_files(src_dir);
    }
    if(!inc_opt.has_value())
    {
      std::cout << "[WARNING] No include directory found.\n";
    }
    else
    {
      fs::path include_dir = inc_opt.value();
      header_files = get_header_files(include_dir);
    }
    
    std::vector< node* > dag_nodes = create_node_vec(std::move(src_files),
                                                     std::move(header_files),
                                                     std::move(obj_files));
    return dag(dag_nodes);
  }
}
