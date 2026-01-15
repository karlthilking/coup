/* json_test.cxx */
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include "../include/coup_json.hxx"

#define GOOD_CONFIG "../tests/config_examples/good_config.json"
#define BAD_CONFIG "../tests/config_examples/bad_config.json"

namespace fs = std::filesystem;
using namespace coup;

class test_json : public testing::Test
{
protected:
	test_json()
	{
	}
	~test_json()
	{
	}

	void SetUp() override
	{
	}
	void TearDown() override
	{
	}

	coup_json good;
	coup_json bad;
};

TEST_F(test_json, test_constructor)
{
	EXPECT_NO_THROW(good = coup_json(fs::path(GOOD_CONFIG)));
	std::cout << good.dump(4);

	EXPECT_THROW(bad = coup_json(fs::path(BAD_CONFIG)), std::runtime_error);
}

TEST_F(test_json, get_cpp_version)
{
	good = coup_json(fs::path(GOOD_CONFIG));
	EXPECT_EQ(good.get_cpp_version(), "c++20");
}

TEST_F(test_json, get_compiler)
{
	good = coup_json(fs::path(GOOD_CONFIG));
	EXPECT_EQ(good.get_compiler(), "gcc");
}

TEST_F(test_json, get_executable)
{
	good = coup_json(fs::path(GOOD_CONFIG));
	EXPECT_EQ(good.get_executable(), "my_project");
}

TEST_F(test_json, get_source_directories)
{
	good = coup_json(fs::path(GOOD_CONFIG));
	std::vector<std::string> sources = good.get_source_directories();

	EXPECT_EQ(sources.size(), 1);
	EXPECT_EQ(sources.front(), "src");
}

TEST_F(test_json, get_build_directory)
{
	good = coup_json(fs::path(GOOD_CONFIG));
	EXPECT_EQ(good.get_build_directory(), "build");
}

TEST_F(test_json, get_compile_flags)
{
	good = coup_json(fs::path(GOOD_CONFIG));
	std::vector<std::string> compile_flags = good.get_compile_flags();

	auto find_wall =
		std::find(begin(compile_flags), end(compile_flags), "-Wall");
	auto find_wextra =
		std::find(begin(compile_flags), end(compile_flags), "-Wextra");
	auto find_wpedantic =
		std::find(begin(compile_flags), end(compile_flags), "-Wpedantic");

	EXPECT_TRUE(find_wall != compile_flags.end());
	EXPECT_TRUE(find_wextra != compile_flags.end());
	EXPECT_TRUE(find_wpedantic != compile_flags.end());
}
