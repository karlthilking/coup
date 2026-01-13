#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>
#include <vector>

namespace coup
{
class coup_json
{
private:
    nlohmann::json config;

    template<typename K, V>
    V get_entry_or(K key, V value) const noexcept
    {
        if (config.contains(key))
            return config[key];
        else
            return value;
    }

    bool meets_required() const noexcept;

    [[noreturn]] void config_error();
public:
    coup_json(const fs::path& config_file);
    
    std::string get_compiler() const noexcept;

    std::string get_cpp_version() const noexcept;

    std::string get_executable() const noexcept;

    std::vector<std::string>
    get_source_directories() const noexcept;

    std::string get_build_directory() const noexcept;

    std::vector<std::string>
    get_source_files() const noexcept;

    std::vector<std::string>
    get_compile_flags() const noexcept;
};

} // namespace coup
