#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>
#include "../include/coup_filesystem.hxx"

namespace fs = std::filesystem;
using namespace coup;

class test_filesystem : public testing::Test {
protected:
  test_filesystem() = default;
  ~test_filesystem() = default;

  void SetUp() override {}

  fs::path root_dir = get_root_dir();
  fs::path src_dir = get_src_dir(root_dir);
  fs::path include_dir = get_include_dir(root_dir);
  fs::path out_dir = get_out_dir(root_dir);

  std::vector<fs::path> src_files;
  std::vector<fs::path> header_files;
  std::vector<fs::path> obj_files;
};

TEST_F(test_filesystem, root_dir_test) {
  try {
    root_dir = get_root_dir();

    EXPECT_TRUE(fs::exists(root_dir));
    EXPECT_FALSE(root_dir.empty());

    std::cout << "Root Directory: " << root_dir.string() << "\n";
  } catch (const std::runtime_error& e) {
    std::cout << "get_root_dir() failed\n";
  }
}

TEST_F(test_filesystem, src_dir_test) {
  src_dir = get_src_dir(root_dir);

  EXPECT_TRUE(fs::exists(src_dir));
  EXPECT_FALSE(src_dir.empty());
  
  std::cout << "Source Directory: " << src_dir.string() << "\n";
}

TEST_F(test_filesystem, src_files_test) {
  src_files = find_src_files(src_dir);
  
  std::cout << "Source Files:\n";
  for (const fs::path& src : src_files) {
    EXPECT_TRUE(fs::exists(src));
    EXPECT_FALSE(src.empty());

    std::cout << src.string() << "\n";
  }
}

TEST_F(test_filesystem, include_dir_test) {
  include_dir = get_include_dir(root_dir);

  EXPECT_TRUE(fs::exists(include_dir));
  EXPECT_FALSE(include_dir.empty());

  std::cout << "Include Directory: " << include_dir.string() << "\n";
}

TEST_F(test_filesystem, header_files_test) {
  header_files = find_header_files(include_dir);
  
  std::cout << "Header Files:\n";
  for (const fs::path& header : header_files) {
    EXPECT_TRUE(fs::exists(header));
    EXPECT_FALSE(header.empty());

    std::cout << header.string() << "\n";
  }
}

TEST_F(test_filesystem, out_dir_test) {
  out_dir = get_out_dir(root_dir);

  EXPECT_TRUE(fs::exists(out_dir));
  EXPECT_FALSE(out_dir.empty());

  std::cout << "Build Directory: " << out_dir.string() << "\n";
}

TEST_F(test_filesystem, obj_files_test) {
  obj_files = get_obj_files(out_dir);

  std::cout << "Object Files:\n";
  for (const fs::path& obj : obj_files) {
    EXPECT_TRUE(fs::exists(obj));
    EXPECT_FALSE(obj.empty());

    std::cout << obj.string() << "\n";
  }
}

