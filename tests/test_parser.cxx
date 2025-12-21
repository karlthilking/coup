#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include "../source/parser.hxx"

using namespace coup;

class parser_test : public testing::Test
{
protected:
	parser_test() = default;
	~parser_test() = default;
	
	std::string s1 = "main.cpp";
	std::string s2 = "test.cxx";
	std::string s3 = "utils.hxx";
	std::string s4 = "math.h";
	std::string s5 = "file.hpp";
	std::string s6 = "file.c++";
	std::string s7 = "file.cc";
	std::string s8 = "file.C";
	std::string s9 = "file.hh";

	void SetUp() override {}
};

TEST_F(parser_test, get_stem_cpp) {	EXPECT_EQ(get_stem(s1), "main");}

TEST_F(parser_test, get_stem_cxx) {	EXPECT_EQ(get_stem(s2), "test");}

TEST_F(parser_test, get_stem_cplusplus) {	EXPECT_EQ(get_stem(s6), "file");}

TEST_F(parser_test, get_stem_cc) { EXPECT_EQ(get_stem(s7), "file");}

TEST_F(parser_test, get_stem_C) { EXPECT_EQ(get_stem(s8), "file");}

TEST_F(parser_test, get_stem_hxx) {	EXPECT_EQ(get_stem(s3), "utils");}

TEST_F(parser_test, get_stem_h) {	EXPECT_EQ(get_stem(s4), "math");}

TEST_F(parser_test, get_stem_hpp) {	EXPECT_EQ(get_stem(s5), "file");}

TEST_F(parser_test, get_stem_hh) { EXPECT_EQ(get_stem(s9), "file");}

TEST_F(parser_test, get_ext_cpp) { EXPECT_EQ(get_extension(s1), "cpp");}

TEST_F(parser_test, get_ext_cxx) { EXPECT_EQ(get_extension(s2), "cxx");}

TEST_F(parser_test, get_ext_cplusplus) { EXPECT_EQ(get_extension(s6), "c++");}

TEST_F(parser_test, get_ext_cc) { EXPECT_EQ(get_extension(s7), "cc");}

TEST_F(parser_test, get_ext_C) { EXPECT_EQ(get_extension(s8), "C");}

TEST_F(parser_test, get_ext_hxx) { EXPECT_EQ(get_extension(s3), "hxx");}

TEST_F(parser_test, get_ext_h) { EXPECT_EQ(get_extension(s4), "h");}

TEST_F(parser_test, get_ext_hpp) { EXPECT_EQ(get_extension(s5), "hpp");}

TEST_F(parser_test, get_ext_hh) { EXPECT_EQ(get_extension(s9), "hh");}
