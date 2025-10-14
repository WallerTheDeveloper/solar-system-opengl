//
// Created by Daniel on 30-Sep-25.
//

#ifndef PLANET_FACTORY_H
#define PLANET_FACTORY_H

#include <CelestialBodyTypes.h>

#include <memory>
#include <vector>

class CelestialBody;
class BufferManager;
class MeshGenerator;
class TextureManager;

struct CelestialBodyConfig {
  BodyType type;
  float mass;
  float radius;
  float orbitalRadius;
  float eccentricity;
  float orbitalPeriod;
  float rotationAngle;
  glm::vec3 position;
  glm::vec3 velocity;
  const char* texturePath;
  bool hasRing = false;
};

class CelestialBodyFactory {
public:
  static std::vector<std::unique_ptr<CelestialBody>> createSolarSystem(
      BufferManager& bufferManager, MeshGenerator& meshGenerator,
      TextureManager& textureManager);

  static float getRotationSpeed(BodyType type);
  static glm::vec3 getScale(BodyType type);
  static glm::vec3 getRotationAxis(BodyType type);

private:
  static std::vector<CelestialBodyConfig> getSolarSystemConfig();
};

#endif // PLANET_FACTORY_H