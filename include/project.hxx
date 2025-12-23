#include <string>
#include <vector>

#include "file_tracker.hxx"

namespace coup
{
  class project
  {
  private:
    std::vector< std::string > src_files;
    std::vector< std::string > header_files;
  public:
    project(std::vector< std::string > sources, 
            std::vector< std::string > headers)
      : src_files(sources), header_files(headers)
    {}

    [[nodiscard]] const std::vector< std::string>&
    get_src_files() const noexcept 
    {
      return src_files;
    }

    [[nodiscard]] const std::vector< std::string >&
    get_header_files() const noexcept
    {
      return header_files;
    }
  };
}
