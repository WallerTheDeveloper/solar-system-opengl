//
// Created by Daniel on 04-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_APPCONFIG_H
#define SOLAR_SYSTEM_OPENGL_APPCONFIG_H

#include <string>
#include <vector>

#include "core/ResourcePath.h"

struct AppConfig {
  static constexpr const char* WINDOW_NAME = "Solar System Simulation";
  static constexpr bool ENABLE_GL_DEPTH_TEST = true;
  static constexpr float TIME_SCALE = 1.0f;
  static constexpr unsigned int SCR_WIDTH = 1920;
  static constexpr  unsigned int SCR_HEIGHT = 1080;
  static constexpr float DISTANCE_SCALE_FACTOR = 0.1f;

  static const std::vector<std::string>& getSkyboxFaces() {
    static const std::vector faces = {
      ResourcePath::get("textures/skybox1.png"),
      ResourcePath::get("textures/skybox2.png"),
      ResourcePath::get("textures/skybox3.png"),
      ResourcePath::get("textures/skybox4.png"),
      ResourcePath::get("textures/skybox5.png"),
      ResourcePath::get("textures/skybox6.png")
    };
    return faces;
  }
};

#endif  // SOLAR_SYSTEM_OPENGL_APPCONFIG_H
