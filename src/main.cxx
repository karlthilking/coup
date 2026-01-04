#include <iostream>
#include <stdexcept>
#include <string>
#include <chrono>
#include "../include/coup_system.hxx"
#include "../include/coup_file.hxx"
#include "../include/coup_project.hxx"
#include "../include/coup_logger.hxx"

using namespace coup;
int main(int argc, char* argv[]) {
  if (argc < 2) {
    coup_logger::print_usage();
    return -1;
  }

  coup_project proj = coup_project::make_project();
}
