#include <iostream>

#include "SolarSystemApp.h"

int main() {
  SolarSystemApp app;

  try {
    std::cout << "Initializing Solar System Application...\n";
    app.initialize();

    std::cout << "✓ Solar System Application initialized successfully!\n";

    app.run();
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return -1;
  }

  std::cout << "Application terminated successfully\n";
  return 0;
}