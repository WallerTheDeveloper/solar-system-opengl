#include <iostream>

#include "SolarSystemApp.h"

int main() {
  try {
    SolarSystemApp app;
    if (!app.initialize()) {
      std::cout << "App initialization failed" << std::endl;
      return -1;
    }

    app.run();
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return -1;
  }

  std::cout << "Application terminated successfully\n";
  return 0;
}