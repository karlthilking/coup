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
  
  std::string local_include_normal = "#include /"file.h/"";
  std::string local_include_no_space = "#include/"file.h/"";
  std::string local_include_multiple_spaces = "#include    /"file.h/"";
  std::string system_include = "#include <iostream>";
  std::string non_include = "int x = 42;";
  std::string incorrect_include = "#include /"file.h";

  void SetUp override {}
};

TEST_F(regex_test, test_local_include_normal)
{
  std::optional< std::string_view > h_opt = extract_header(local_include_normal);
  EXPECT_TRUE(h_opt.has_value());
  
  std::string_view header = h_opt.value();
  EXPECT_EQ(header, "file.h");
}

TEST_F(regex_test, test_local_include_no_space)
{
  std::optional< std::string_view > h_opt = extract_header(local_include_no_space);
  EXPECT_TRUE(h_opt.has_value());

  std::string_view header = h_opt.value();
  EXPECT_EQ(header, "file.h");
}

TEST_F(regex_test, test_local_include_multiple_spaces)
{
  std::optional< std::string_view > h_opt = extract_header(local_include_multiple_spaces);
  EXPECT_TRUE(h_opt.has_value());

  std::string_view header = h_opt.value();
  EXPECT_EQ(header, "file.h");
}

TEST_F(regex_test, test_system_include)
{
  std::optional< std::string_view > h_opt = extract_header(system_include);
  EXPECT_TRUE(!h_opt.has_value());
}

TEST_F(regex_test, test_non_include)
{
  std::optional< std::string_view > h_opt = extract_header(non_include);
  EXPECT_TRUE(!h_opt.has_value());
}

TEST_F(regex_test, test_incorrect_include)
{
  std::optional< std::string_view > h_opt = extract_header(incorrect_include);
  EXPECT_TRUE(!h_opt.has_value());
}

