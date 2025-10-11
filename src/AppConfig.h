//
// Created by Daniel on 04-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_APPCONFIG_H
#define SOLAR_SYSTEM_OPENGL_APPCONFIG_H

#include <string>

struct AppConfig {
  static constexpr const char* WINDOW_NAME = "Solar System Simulation";
  static constexpr bool ENABLE_GL_DEPTH_TEST = true;
  static constexpr float TIME_SCALE = 1.5f;
  static constexpr unsigned int SCR_WIDTH = 1920;
  static constexpr  unsigned int SCR_HEIGHT = 1080;
  static const std::vector<std::string> SKYBOX_FACES;
};

#endif  // SOLAR_SYSTEM_OPENGL_APPCONFIG_H
