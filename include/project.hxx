#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include "file_tracker.hxx"

namespace fs = std::filesystem;

namespace coup
{ 
  project create_project()
  {
    std::optional< fs::path > root_opt = get_root();
    assert(root_opt.has_value());

    fs::path root = root_opt.value();

    std::optional< fs::path > src_dir_opt = get_src_dir(root);
    std::optional< fs::path > inc_dir_opt = get_include_dir(root);

    if(!(src_dir_opt.has_value() || inc_dir_opt.has_value())
    {
      std::runtime_error("[ERROR] No source or include directory provided\n");
    }
    else if(src_dir_opt.has_value() && inc_dir_opt.has_value())
    {
      fs::path src_dir = src_dir_opt.value();
      fs::path include_dir = inc_dir_opt.value();

      std::vector< fs::path > src_files = get_src_files(src_dir);
      std::vector< fs::path > header_files = get_header_files(include_dir);

      return project(src_files, header_files);
    }
    else if(src_dir_opt.has_value())
    {
      fs::path src_dir = src_dir_opt.value();
      std::vector< fs::path > src_files = get_src_files(src_dir);

      return project(src_files);
    }
    else
    {
      fs::path include_dir = inc_dir_opt.value();
      std::vector< fs::path > header_files = get_header_files(include_dir);

      return project({}, header_files);
    }
  }

  class project
  {
  private:
    std::vector< fs::path > src_files;
    std::vector< fs::path > header_files;
  public:
    project(const std::vector< fs::path >& sources = {}, 
            const std::vector< fs::path >& headers = {});

    [[nodiscard]]
    const std::vector< fs::path >& get_src_files() const noexcept;

    [[nodiscard]]
    const std::vector< fs::path >& get_header_files() const noexcept;

  };
}
