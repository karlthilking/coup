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
	
	std::string tc1 = "main.cpp";
	std::string tc2 = "main.cxx";
	std::string tc3 = "test.h";
	std::string tc4 = "test.hpp";
	std::string tc5 = "test.hxx";
	std::string tc6 = "a";
	std::string tc7 = ".txt";
	void SetUp() override {}
};

TEST_F(parser_test, get_stem1)
{
	std::optional< std::string_view > opt = get_stem(tc1);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "main");
}
TEST_F(parser_test, get_stem2)
{
	std::optional< std::string_view > opt = get_stem(tc2);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "main");
}
TEST_F(parser_test, get_stem3)
{
	std::optional< std::string_view > opt = get_stem(tc3);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "test");
}
TEST_F(parser_test, get_stem4)
{
	std::optional< std::string_view > opt = get_stem(tc4);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "test");
}
TEST_F(parser_test, get_stem5)
{
	std::optional< std::string_view > opt = get_stem(tc5);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "test");
}
TEST_F(parser_test, get_stem6)
{
	std::optional< std::string_view > opt = get_stem(tc6);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "a");
}
TEST_F(parser_test, get_stem7)
{
	std::optional< std::string_view > opt = get_stem(tc7);
	EXPECT_FALSE(opt.has_value());
}


TEST_F(parser_test, get_ext1)
{
	std::optional< std::string_view > opt = get_extension(tc1);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "cpp");
}
TEST_F(parser_test, get_ext2)
{
	std::optional< std::string_view > opt = get_extension(tc2);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "cxx");
}
TEST_F(parser_test, get_ext3)
{
	std::optional< std::string_view > opt = get_extension(tc3);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "h");
}
TEST_F(parser_test, get_ext4)
{
	std::optional< std::string_view > opt = get_extension(tc4);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "hpp");
}
TEST_F(parser_test, get_ext5)
{
	std::optional< std::string_view > opt = get_extension(tc5);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "hxx");
}
TEST_F(parser_test, get_ext6)
{
	std::optional< std::string_view > opt = get_extension(tc6);
	EXPECT_FALSE(opt.has_value());
}
TEST_F(parser_test, get_ext7)
{
	std::optional< std::string_view > opt = get_extension(tc7);
	EXPECT_TRUE(opt.has_value());
	EXPECT_EQ(opt.value(), "txt");
}


TEST_F(parser_test, get_obj_file)
{
	
}
