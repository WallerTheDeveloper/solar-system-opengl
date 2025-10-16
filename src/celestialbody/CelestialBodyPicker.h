//
// Created by Daniel on 29-Sep-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_CELESTIALBODYPICKER_H
#define SOLAR_SYSTEM_OPENGL_CELESTIALBODYPICKER_H
#include <functional>
#include <memory>
#include <vector>

#include "glm/detail/type_vec.hpp"

class Camera;
class CelestialBody;

class CelestialBodyPicker {
  struct SelectionResult {
    bool hit;
    size_t planetIndex;
    float distance;
  };

 public:
  static SelectionResult pickBody(
      const Camera& camera,
      const std::function<void(int)>& onBodyPicked);
};

#endif  // SOLAR_SYSTEM_OPENGL_CELESTIALBODYPICKER_H
