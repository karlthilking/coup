#include <vector>
#include <filesystem>
#include <unordered_map>
#include <queue>
#include <cassert>

#include "#../include/dag.hxx"

namespace coup;
namespace fs = std::filesystem;

dag::dag(std::vector< node* > S) noexcept
  : nodes(S),
{}

dag::~dag() noexcept
{
  for(node* u: nodes)
  {
    assert(u != nullptr);
    delete(u);
  }
}

std::vector< fs::path > dag::topological_sort() const noexcept
{
  std::unordered_map< node*, int > indegree;
  for(const node* u: nodes)
  {
    for(node* v: u->children)
    {
      indegree[v]++;
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
  return toplogical_order;
}

node::node(file_type ft, const fs::path& p) noexcept
  : type(ft), 
    path(p),
{}

void node::insert(node* child) noexcept
{
  children.push_back(child);
}

void node::insert(file_type ft, const fs::path& p) noexcept
{
  children.emplace_back(new node(ft, p));
}

fs::file_time_type node::last_update() const noexcept
{
  return fs::last_write_time(path);
}
