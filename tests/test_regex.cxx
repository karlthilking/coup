#include <gtest/gtest.h>

#include <string>
#include <optional>
#include <string_view>

#include "../include/regex.hxx"

using namespace coup;

class regex_test : public testing::Test
{
protected:
  regex_test() = default;
  ~regex_test() = default;
  
  void SetUp() override {}

  std::string s1 = "#include \"header.hpp\""; 
  std::string s2 = "#include\"header.hpp\"";
  std::string s3 = "#include     \"header.hpp\"";
  std::string s4 = "#include <iostream>";
  std::string s5 = "int x = 42";
};

TEST_F(regex_test, header_test)
{
  std::optional< std::string_view > opt = extract_header(s1);
  EXPECT_TRUE(opt.has_value());

  std::string_view header = opt.value();
  EXPECT_EQ(header, "header.hpp");
}

TEST_F(regex_test, no_spaces_test)
{
  std::optional< std::string_view > opt = extract_header(s2);
  EXPECT_TRUE(opt.has_value());

  std::string_view header = opt.value();
  EXPECT_EQ(header, "header.hpp");
}

TEST_F(regex_test, multiple_spaces_test)
{
  std::optional< std::string_view > opt = extract_header(s3);
  EXPECT_TRUE(opt.has_value());

  std::string_view header = opt.value();
  EXPECT_EQ(header, "header.hpp");
}

TEST_F(regex_test, system_header_test)
{
  std::optional< std::string_view > opt = extract_header(s4);
  EXPECT_FALSE(opt.has_value());
}

TEST_F(regex_test, non_include_test)
{
  std::optional< std::string_view > opt = extract_header(s5);
  EXPECT_FALSE(opt.has_value());
}




