#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <optional>
#include <vector>
#include <filesystem>

#include "../include/parser.hxx"
#include "../include/file_tracker.hxx"

using namespace coup;
namespace fs = std::filesystem;

class parser_test : public testing::Test
{
protected:
	parser_test() = default;
	~parser_test() = default;
	
	std::string tc1 = "main.cpp";
	std::string tc2 = "main.cxx";
	std::string tc3 = "test.h";
	std::string tc4 = "test.hpp";
	std::string tc5 = "test.hxx";
	std::string tc6 = "a";
	std::string tc7 = ".txt";

  std::string fp1 = "src/main.cpp";
  std::string fp2 = "include/header.h";
  std::string fp3 = "CMakeLists.txt";
  std::string fp4 = "tests/test_file.cpp";

	void SetUp() override {}
};

TEST_F(parser_test, get_stem_cpp)
{
	std::optional< std::string_view > opt = get_stem(tc1);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "main");
}
TEST_F(parser_test, get_stem_cxx)
{
	std::optional< std::string_view > opt = get_stem(tc2);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "main");
}
TEST_F(parser_test, get_stem_h)
{
	std::optional< std::string_view > opt = get_stem(tc3);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "test");
}
TEST_F(parser_test, get_stem_hpp)
{
	std::optional< std::string_view > opt = get_stem(tc4);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "test");
}
TEST_F(parser_test, get_stem_hxx)
{
	std::optional< std::string_view > opt = get_stem(tc5);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "test");
}
TEST_F(parser_test, get_stem_a)
{
	std::optional< std::string_view > opt = get_stem(tc6);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "a");
}
TEST_F(parser_test, get_stem_blank)
{
	std::optional< std::string_view > opt = get_stem(tc7);
	EXPECT_FALSE(opt.has_value());
}

TEST_F(parser_test, get_ext_cpp)
{
	std::optional< std::string_view > opt = get_extension(tc1);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "cpp");
}
TEST_F(parser_test, get_ext_cxx)
{
	std::optional< std::string_view > opt = get_extension(tc2);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "cxx");
}
TEST_F(parser_test, get_ext_h)
{
	std::optional< std::string_view > opt = get_extension(tc3);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "h");
}
TEST_F(parser_test, get_ext_hpp)
{
	std::optional< std::string_view > opt = get_extension(tc4);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "hpp");
}
TEST_F(parser_test, get_ext_hxx)
{
	std::optional< std::string_view > opt = get_extension(tc5);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "hxx");
}
TEST_F(parser_test, get_ext_none)
{
	std::optional< std::string_view > opt = get_extension(tc6);
	EXPECT_FALSE(opt.has_value());
}
TEST_F(parser_test, get_ext_txt)
{
	std::optional< std::string_view > opt = get_extension(tc7);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "txt");
}

TEST_F(parser_test, create_obj_file_main)
{
  std::string obj_file = create_obj_file(tc1);
  EXPECT_EQ(obj_file, "main.o");
}

TEST_F(parser_test, create_obj_file_srcmain)
{
  std::string obj_file = create_obj_file(fp1);
  EXPECT_EQ(obj_file, "src/main.o");
}

TEST_F(parser_test, get_filename_main)
{
  std::optional< std::string_view > opt = get_filename(fp1);
  EXPECT_TRUE(opt.has_value());

  std::string_view filename = opt.value();
  EXPECT_EQ(filename, "main.cpp");
}

TEST_F(parser_test, get_filename_header)
{
  std::optional< std::string_view > opt = get_filename(fp2);
  EXPECT_TRUE(opt.has_value());

  std::string_view filename = opt.value();
  EXPECT_EQ(filename, "header.h");
}

TEST_F(parser_test, get_filename_cmake)
{
  std::optional< std::string_view > opt = get_filename(fp3);
  EXPECT_TRUE(opt.has_value());

  std::string_view filename = opt.value();
  EXPECT_EQ(filename, "CMakeLists.txt");
}

TEST_F(parser_test, get_filename_test)
{
  std::optional< std::string_view > opt = get_filename(fp4);
  EXPECT_TRUE(opt.has_value());

  std::string_view filename = opt.value();
  EXPECT_EQ(filename, "test_file.cpp");
}

TEST_F(parser_test, get_filename_none)
{
  std::string empty = "";
  std::optional< std::string_view > opt = get_filename(empty);
  EXPECT_FALSE(opt.has_value());
}

TEST_F(parser_test, test_abs_src_getter)
{
  std::optional< fs::path > r_opt = get_root();
  fs::path root = r_opt.value();
  
  std::optional< fs::path > src_dir_opt = get_src_dir(root);
  fs::path src_dir = src_dir_opt.value();

  std::vector< fs::path > src_files = get_src_files(src_dir);
  for(const fs::path& src: src_files)
  {
    std::string src_path = src.string();
    std::optional< std::string_view > src_name_opt = get_filename(src_path);
    std::string_view src_name = src_name_opt.value();

    fs::path src_abs_path = get_abs_src(src_name, src_dir);
    EXPECT_TRUE(fs::exists(src_abs_path));
  }
}

TEST_F(parser_test, test_abs_header_getter)
{
  std::optional< fs::path > r_opt = get_root();
  fs::path root = r_opt.value();

  std::optional< fs::path > include_dir_opt = get_include_dir(root);
  fs::path include_dir = include_dir_opt.value();

  std::vector< fs::path > header_files = get_header_files(include_dir);
  for(const fs::path& header: header_files)
  {
    std::string header_path = header.string();
    std::optional< std::string_view > header_name_opt = get_filename(header_path);
    std::string_view header_name = header_name_opt.value();

    fs::path header_abs_path = get_abs_header(header_name, include_dir);
    EXPECT_TRUE(fs::exists(header_abs_path));
  }
}
