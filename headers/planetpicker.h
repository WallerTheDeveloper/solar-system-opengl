//
// Created by Daniel on 29-Sep-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_PLANETPICKER_H
#define SOLAR_SYSTEM_OPENGL_PLANETPICKER_H

#include <glm/glm.hpp>
#include <vector>
#include "planet.h"
#include "camera.h"

class PlanetPicker {
public:
  struct SelectionResult {
    bool hit;
    size_t planetIndex;
    float distance;
  };

  static SelectionResult pickPlanet(const Camera& camera,
                                   const std::vector<Planet>& planets,
                                   const std::vector<glm::vec3>& scales);

private:
  static bool raySphereIntersection(const glm::vec3& rayOrigin,
                                   const glm::vec3& rayDirection,
                                   const glm::vec3& sphereCenter,
                                   float sphereRadius,
                                   float& distance);
};


#endif  // SOLAR_SYSTEM_OPENGL_PLANETPICKER_H
