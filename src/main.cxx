#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../include/coup_file.hxx"
#include "../include/coup_logger.hxx"
#include "../include/coup_project.hxx"
#include "../include/coup_system.hxx"

using namespace coup;
int main(int argc, char* argv[]) {
  if (argc < 2) {
    coup_logger::print_usage();
    return -1;
  }

  coup_project proj = coup_project::make_project();
}
