//
// Created by Daniel on 30-Sep-25.
//

#ifndef PLANET_FACTORY_H
#define PLANET_FACTORY_H

#include <memory>
#include <vector>
#include "planet.h"
#include "engine.h"

struct PlanetConfig {
  CelestialBody::BodyType type;
  float mass;
  float radius;
  float orbitalRadius;
  float eccentricity;
  float orbitalPeriod;
  float rotationAngle;
  glm::vec3 position;
  glm::vec3 velocity;
  const char* texturePath;
};

class PlanetFactory {
public:
  static std::vector<Planet> createSolarSystem(Engine* engine);

  static float getRotationSpeed(CelestialBody::BodyType type);
  static glm::vec3 getScale(CelestialBody::BodyType type);
  static glm::vec3 getRotationAxis(CelestialBody::BodyType type);

private:
  static std::vector<PlanetConfig> getSolarSystemConfig();
};

#endif // PLANET_FACTORY_H