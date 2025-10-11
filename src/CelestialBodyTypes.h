//
// Created by Daniel on 08-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_BODYTYPE_H
#define SOLAR_SYSTEM_OPENGL_BODYTYPE_H

#include "glm/detail/type_vec3.hpp"

enum BodyType {
  Sun,
  Mercury,
  Venus,
  Earth,
  Mars,
  Jupiter,
  Saturn,
  Uranus,
  Neptune
};

struct BodyProps {
  BodyType type;
  float mass;           // kg
  float radius;         // meters (for rendering scale)
  float semiMajorAxis;  // meters (orbital radius)
  float eccentricity;   // 0 = circle, <1 = ellipse
  float orbitalPeriod;  // seconds
  float currentAngle;   // current position in orbit (radians)
  glm::vec3 position;
  glm::vec3 velocity;
};

#endif  // SOLAR_SYSTEM_OPENGL_BODYTYPE_H
