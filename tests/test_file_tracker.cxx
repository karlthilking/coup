#include <gtest/gtest.h>
#include <optional>
#include <filesystem>
#include <string>
#include <vector>
#include <string_view>
#include <iostream>

#include "../include/file_tracker.hxx"
#include "../include/parser.hxx"

namespace fs = std::filesystem;
using namespace coup;

class file_tracker_test : public testing::Test
{
protected:
	file_tracker_test() = default;
	~file_tracker_test() = default;
		
	void SetUp() override {}
  
  fs::path s = "src/main.cpp";
  fs::path h = "include/header.hpp";
  fs::path o = "build/main.o";
};

TEST_F(file_tracker_test, get_root_test)
{
  std::optional< fs::path > r_opt = get_root();
  EXPECT_TRUE(r_opt.has_value());

  fs::path root = r_opt.value();
  EXPECT_TRUE(fs::exists(root));
  EXPECT_FALSE(root.empty());
  
  std::cout << "[ROOT]:\n" << root.string() << "\n";
}

TEST_F(file_tracker_test, get_src_dir_test)
{
  std::optional< fs::path > r_opt = get_root();
  fs::path root = r_opt.value();

  std::optional< fs::path > src_dir_opt = get_src_dir(root);
  EXPECT_TRUE(src_dir_opt.has_value());
  
  fs::path src_dir = src_dir_opt.value();
  EXPECT_TRUE(fs::exists(src_dir));
  EXPECT_FALSE(src_dir.empty());

  std::cout << "[SRC]:\n" << src_dir.string() << "\n";
}

TEST_F(file_tracker_test, get_include_dir_test)
{
  std::optional< fs::path > r_opt = get_root();
  fs::path root = r_opt.value();

  std::optional< fs::path > inc_dir_opt = get_include_dir(root);
  EXPECT_TRUE(inc_dir_opt.has_value());

  fs::path include_dir = inc_dir_opt.value();
  EXPECT_TRUE(fs::exists(include_dir));
  EXPECT_FALSE(include_dir.empty());

  std::cout << "[INCLUDE]:\n" << include_dir.string() << "\n";
}

TEST_F(file_tracker_test, get_src_files_test)
{
  std::optional< fs::path > r_opt = get_root();
  fs::path root = r_opt.value();

  std::optional< fs::path > src_dir_opt = get_src_dir(root);
  fs::path src_dir = src_dir_opt.value();

  std::vector< fs::path > src_files = get_src_files(src_dir);
  EXPECT_FALSE(src_files.empty());
    
  std::cout << "[Source Files]:\n";
  for(const fs::path& src: src_files)
  {
    std::cout << src.string() << "\n";
    EXPECT_TRUE(src.parent_path() == src_dir);
    EXPECT_TRUE(fs::exists(src));
  }
}

TEST_F(file_tracker_test, get_header_files_test)
{
  std::optional< fs::path > r_opt = get_root();
  fs::path root = r_opt.value();

  std::optional< fs::path > inc_dir_opt = get_include_dir(root);
  fs::path inc_dir = inc_dir_opt.value();

  std::vector< fs::path > header_files = get_header_files(inc_dir);
  EXPECT_FALSE(header_files.empty());
  
  std::cout << "[Header Files]:\n";
  for(const fs::path& header: header_files)
  {
    std::cout << header.string() << "\n";
    EXPECT_TRUE(header.parent_path() == inc_dir);
    EXPECT_TRUE(fs::exists(header));
  }
}

TEST_F(file_tracker_test, get_obj_files_test)
{
  std::optional< fs::path > r_opt = get_root();
  fs::path root = r_opt.value();

  std::vector< fs::path > obj_files = get_obj_files(root);

  std::cout << "[Object Files]:\n";
  for(const fs::path& obj: obj_files)
  { 
    EXPECT_TRUE(fs::exists(obj));
    std::cout << obj.string();
  }
}

TEST_F(file_tracker_test, is_src_file_true_test)
{
  EXPECT_TRUE(is_src_file(s));
}
TEST_F(file_tracker_test, is_src_file_false_test)
{
  EXPECT_FALSE(is_src_file(h));
  EXPECT_FALSE(is_src_file(o));
}

TEST_F(file_tracker_test, is_header_file_true_test)
{
  
  EXPECT_TRUE(is_header_file(h));
}

TEST_F(file_tracker_test, is_header_file_false_test)
{
  EXPECT_FALSE(is_header_file(s));
  EXPECT_FALSE(is_header_file(o));
}

TEST_F(file_tracker_test, is_obj_file_true_test)
{
  EXPECT_TRUE(is_obj_file(o));
}

TEST_F(file_tracker_test, is_obj_file_false_test)
{
  EXPECT_FALSE(is_obj_file(s));
  EXPECT_FALSE(is_obj_file(h));
}





		

