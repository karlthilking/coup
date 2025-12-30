#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>

#include "../include/commands.hxx" // build_command, run_command, clean_command, command, execute, create_command

using namespace coup;

int main(int argc, char** argv)
{
	if(argc < 1)
  {
    return 1;
  }

  std::string cmd_str = argv[1];
  command_type cmd = create_command(cmd_str);
  execute_cmd(cmd);
  
	return 0;
}

