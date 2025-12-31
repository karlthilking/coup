#include <cstdlib>
#include <string>

#include "../include/commands.hxx"  // command_type, build_command, run_command, clean_command, execute_cmd, create_command

int main(int argc, char** argv) {
  if (argc < 1) {
    return 1;
  }

  std::string cmd_str = argv[1];
  coup::command_type cmd = coup::create_command(cmd_str);
  bool result = coup::execute_cmd(cmd);

  return 0;
}
