#include <gtest/gtest.h>
#include <string>
#include <string_view>

#include "../include/parser.hxx"

using namespace coup;

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
