//
// Created by Daniel on 15-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_RAYINTERSECTION_H
#define SOLAR_SYSTEM_OPENGL_RAYINTERSECTION_H

#include "glm/detail/type_vec.hpp"

class RayIntersection {
public:
  static bool raySphereIntersection(const glm::vec3& rayOrigin,
                                   const glm::vec3& rayDirection,
                                   const glm::vec3& sphereCenter,
                                   float sphereRadius,
                                   float& distance);
};

#endif  // SOLAR_SYSTEM_OPENGL_RAYINTERSECTION_H
