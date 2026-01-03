#include <iostream>
#include <stdexcept>
#include <string>

#include "../include/coup_file.hxx"
#include "../include/coup_project.hxx"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    // coup::print_usage();
    return -1;
  }

  try {
    const char* command = argv[1];
    coup::coup_project proj = coup::coup_project::make_project();
    // coup::execute_command(command, proj);
  } catch (const std::runtime_error& e) {
    std::cerr << "[Runtime Error] " << e.what() << "\n";
    return -1;
  } catch (...) {
    std::cerr << "[Fatal Error] Terminating...\n";
    return -1;
  }
  return 0;
}
