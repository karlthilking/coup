#include <iostream>
#include <stdexcept>
#include <string>

constexpr int EXIT_SUCCESS = 0;
constexpr int EXIT_ERROR = 1;
constexpr int EXIT_USAGE_ERROR = 2;
constexpr int EXIT_FATAL = 3;

int main(int argc, char** argv) {
  if (argc < 2) {
    coup::print_usage();
    return EXIT_USAGE_ERROR;
  }

  try {
    const char* arg = argv[1];

    coup::command_type cmd = coup::create_command(arg);
    coup::project proj = coup::project::create_project();

    proj.execute(cmd);
  } catch (const std::runtime_error& e) {
    std::cerr << "[Runtime Error]: " << e.what() << "\n";
    return EXIT_ERROR;
  } catch (const std::invalid_argument& e) {
    std::cerr << "[Invalid Argument]: " << e.what() << "\n";
    return EXIT_USAGE_ERROR;
  } catch (const std::exception& e) {
    std::cerr << "[Error]: " << e.what() << "\n";
    return EXIT_ERROR;
  } catch (...) {
    std::cerr << "[Fatal Error] Exiting...\n";
    return EXIT_FATAL;
  }
  return EXIT_SUCCESS;
}
