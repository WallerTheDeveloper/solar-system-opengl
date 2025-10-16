//
// Created by Daniel on 08-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_BODYTYPE_H
#define SOLAR_SYSTEM_OPENGL_BODYTYPE_H

#include <string>

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
  float currentRotationAngle;   // current position in orbit (radians)
  glm::vec3 position;
  glm::vec3 velocity;
  const char* texturePath;
  bool hasRing = false;
};

struct BodyInfo {
  std::string name;
  float distanceFromSun;  // in AU
  float temperature;      // in Celsius
  std::string type;
  float mass;      // relative to Earth
  float diameter;  // in km
  int moons;
};
#endif  // SOLAR_SYSTEM_OPENGL_BODYTYPE_H
