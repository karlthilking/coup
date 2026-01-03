#include <iostream>
#include <stdexcept>
#include <string>
#include <chrono>
#include "../include/coup_system.hxx"
#include "../include/coup_file.hxx"
#include "../include/coup_project.hxx"

void print_usage() {
  std::cerr << "Usage: ./coup <command>\n"
    << "Commands: build, run, clean\n";
}

int main(int argc, char* argv[]) {
  auto start = std::chrono::high_resolution_clock::now();
  if (argc < 2) {
    print_usage();
    return -1;
  }

  try {
    std::string command = argv[1];
    coup::coup_project proj = coup::coup_project::make_project();
    bool success = coup::execute_command(command);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    if (success) {
      std::cout << "[COUP] " << command << " command executed successfully in " 
        << duration.count() << "ms\n";
    } else {
      std::cerr << "[COUP] " << command << " command failed silently\n";
    }
  } catch (const std::runtime_error& e) {
    std::cerr << "[Runtime Error] " << e.what() << "\n";
    return -1;
  } catch (const std::invalid_argument& e) {
    std::cerr << "[Invalid Argument] " << e.what() << "\n";
    print_usage();
    return -1;
  } catch (...) {
    std::cerr << "[Fatal Error]\nTerminating...\n";
    return -1;
  }
  return 0;
}
