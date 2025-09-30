#include <iostream>

#include "SolarSystemApp.h"

int main() {
  try {
    // Create application
    SolarSystemApp app;

    // Initialize all systems
    if (!app.initialize()) {
      std::cerr << "✗ Failed to initialize application" << std::endl;
      return -1;
    }

    // Run main loop
    app.run();

  } catch (const std::exception& e) {
    std::cerr << "✗ Application error: " << e.what() << std::endl;
    return -1;
  }

  std::cout << "Application terminated successfully" << std::endl;
  return 0;
}