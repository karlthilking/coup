#include <gtest/gtest.h>
#include <variant>
#include <string>
#include <string_view>
#include <vector>
#include <type_traits>

#include "../source/commands.hxx"

using namespace coup;

class commands_test : public testing::Test
{
protected:
	commands_test() = default;
	~commands_test() = default;
	
	std::string build_str = "build";
	std::string run_str = "run";
	std::string clean_str = "clean";

	void SetUp() override {}
};

TEST_F(commands_test, create_build)
{
	command_type m_build = create_command(build_str);
	EXPECT_TRUE(std::holds_alternative< build_command >(m_build));
}
TEST_F(commands_test, create_run)
{
	command_type m_run = create_command(run_str);
	EXPECT_TRUE(std::holds_alternative< run_command >(m_run));
}
TEST_F(commands_test, create_clean)
{
	command_type m_create = create_command(clean_str);
	EXPECT_TRUE(std::holds_alternative< clean_command >(m_create));
}
