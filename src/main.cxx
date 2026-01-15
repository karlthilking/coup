/* main.cxx */
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../include/coup_logger.hxx"
#include "../include/coup_project.hxx"
#include "../include/coup_system.hxx"

using namespace coup;
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		print_usage();
		return -1;
	}

	std::string command = argv[1];
	std::string option;
	if (argc > 2)
	{
		option = argv[2];
	}

	coup_project proj = coup_project::make_project();
	try
	{
		proj.execute_command(command, option);
	}
	catch (const std::exception &e)
	{
		print_usage();
		return -1;
	}
	return 0;
}
