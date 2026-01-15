/* coup_json.hxx */
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace fs = std::filesystem;
namespace coup
{
class coup_json
{
private:
	nlohmann::json config;
	
    // Helper function for getting retrieving the value
	// associated with a certain field in coup_config.json
	// If the field is defined, return the associated value
	// or else return a fallback default value
	template <typename T>
	T get_entry_or(const char *k, T fallback) const noexcept
	{
		if (config.contains(k))
			return config[k];
		else
			return fallback;
	}

	bool meets_required() const noexcept;

	[[noreturn]] void config_error(const char *e);

public:
	coup_json();
	coup_json(const fs::path &config_file);

	std::string get_cpp_version() const noexcept;

	std::string get_compiler() const noexcept;

	std::string get_executable() const noexcept;

	std::vector<std::string> get_source_directories() const noexcept;

	std::string get_build_directory() const noexcept;

	std::vector<std::string> get_compile_flags() const noexcept;

	std::string dump(int tab_width) const noexcept;

    bool contains(const char *key) const noexcept;
};
} // namespace coup
