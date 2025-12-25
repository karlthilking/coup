#include <vector>
#include <filesystem>
#include <unordered_map>
#include <queue>
#include <cassert>
#include <utility>

#include "../include/dag.hxx"

namespace fs = std::filesystem;

namespace coup
{
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

  dag::dag(const std::vector< node* >& S) noexcept
   : nodes(S)
  {}

  dag::dag(std::vector< node* >&& S) noexcept
   : nodes(std::exchange(S, {}))
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
}
