//
// Created by Daniel on 29-Sep-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_PLANETPICKER_H
#define SOLAR_SYSTEM_OPENGL_PLANETPICKER_H
#include <memory>
#include <vector>

#include "glm/detail/type_vec.hpp"

class Camera;
class CelestialBody;

class BodySelectionHandler {
public:
  struct SelectionResult {
    bool hit;
    size_t planetIndex;
    float distance;
  };

  static SelectionResult pickPlanet(const Camera& camera,
                                   const std::vector<std::unique_ptr<CelestialBody>>& celestialBodies,
                                   const std::vector<glm::vec3>& scales);

private:
  static bool raySphereIntersection(const glm::vec3& rayOrigin,
                                   const glm::vec3& rayDirection,
                                   const glm::vec3& sphereCenter,
                                   float sphereRadius,
                                   float& distance);
};


#endif  // SOLAR_SYSTEM_OPENGL_PLANETPICKER_H
