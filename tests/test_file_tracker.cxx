#include <gtest/gtest.h>
#include <optional>
#include <filesystem>
#include <string>
#include <vector>
#include <string_view>
#include <iostream>

#include "../source/file_tracker.hxx"

namespace fs = std::filesystem;
using namespace coup;

class file_tracker_test : public testing::Test
{
protected:
	file_tracker_test() = default;
	~file_tracker_test() = default;
		
	file_tracker ft{};	

	void SetUp() override {}
};

TEST_F(file_tracker_test, test_root)
{
	fs::path root = ft.get_root();
	EXPECT_FALSE(root.empty());
	EXPECT_TRUE(fs::exists(root));

	std::cout << "Root path: " << root.string() << "\n";
}

TEST_F(file_tracker_test, test_file_discovery)
{
	std::vector< fs::path > src_files = ft.get_source_files();
	std::vector< fs::path > header_files = ft.get_header_files();

	EXPECT_TRUE(!(src_files.empty() && header_files.empty()));
	
	std::cout << "Source files:\n";
	for(const auto& src: src_files)
	{
		EXPECT_FALSE(src.empty());
		EXPECT_TRUE(fs::exists(src));
		
		std::cout << src.string() << "\n";
	}

	std::cout << "Header files:\n";
	for(const auto& header: header_files)
	{
		EXPECT_FALSE(header.empty());
		EXPECT_TRUE(fs::exists(header));

		std::cout << header.string() << "\n";
	}
}
		

