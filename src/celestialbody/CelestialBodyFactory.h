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

class CelestialBodyFactory {
 public:
  static void createSolarSystem(BufferManager& bufferManager,
                                MeshGenerator& meshGenerator,
                                TextureManager& textureManager);
  static const std::vector<std::unique_ptr<CelestialBody>>& getCelestialBodies();
  static void clear();

  static std::vector<BodyProps> getSolarSystemConfig();
  static float getRotationSpeed(BodyType type);
  static glm::vec3 getScale(BodyType type);
  static glm::vec3 getRotationAxis(BodyType type);
  static BodyInfo getBodyInfo(BodyType type);
  static BodyProps getBodyProps(BodyType type);

 private:
  static std::vector<std::unique_ptr<CelestialBody>> celestialBodies_;
};

#endif  // PLANET_FACTORY_H