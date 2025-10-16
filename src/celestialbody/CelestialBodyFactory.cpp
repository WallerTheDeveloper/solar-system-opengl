//
// Created by Daniel on 30-Sep-25.
//
#include "CelestialBodyFactory.h"

#include <rendering/renderables/scene/CelestialBody.h>

std::vector<std::unique_ptr<CelestialBody>> CelestialBodyFactory::celestialBodies_;

void CelestialBodyFactory::createSolarSystem(
    BufferManager& bufferManager, MeshGenerator& meshGenerator,
    TextureManager& textureManager) {
  const auto configs = getSolarSystemConfig();

  for (const auto& config : configs) {
    BodyProps bodyProps{config.type,
                        config.mass,
                        config.radius,
                        config.orbitalPeriod,
                        config.eccentricity,
                        config.orbitalPeriod,
                        config.currentRotationAngle,
                        config.position,
                        config.velocity,
                        config.texturePath,
                        config.hasRing};
    celestialBodies_.push_back(
        std::make_unique<CelestialBody>(bodyProps, bufferManager, meshGenerator, textureManager));
  }
}

const std::vector<std::unique_ptr<CelestialBody>>& CelestialBodyFactory::getCelestialBodies() {
  return celestialBodies_;
}

void CelestialBodyFactory::clear() {
  celestialBodies_.clear();
}

std::vector<BodyProps> CelestialBodyFactory::getSolarSystemConfig() {
  return {// Sun
          {Sun, 1.989e30f, 696340000.0f, 0.0f, 0.0f, 0.0f, 0.0f,
           glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
           "../textures/sun.jpg"},
          // Mercury
          {Mercury, 3.301e23f, 2439700.0f, 4.0f, 0.205f, 20.0f, 0.0f,
           glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
           "../textures/mercury.jpg"},
          // Venus
          {Venus, 4.867e24f, 6051800.0f, 6.0f, 0.007f, 35.0f, 0.0f,
           glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
           "../textures/venus.jpg"},
          // Earth
          {Earth, 5.972e24f, 6371000.0f, 8.0f, 0.017f, 50.0f, 0.0f,
           glm::vec3(8.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
           "../textures/earth.jpg"},
          // Mars
          {Mars, 6.417e23f, 3389500.0f, 10.5f, 0.094f, 75.0f, 0.0f,
           glm::vec3(10.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
           "../textures/mars.jpg"},
          // Jupiter
          {Jupiter, 1.898e27f, 69911000.0f, 16.0f, 0.049f, 120.0f, 0.0f,
           glm::vec3(16.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
           "../textures/jupiter.jpg"},
          // Saturn
          {Saturn, 5.683e26f, 58232000.0f, 25.0f, 0.057f, 180.0f,
           0.0f,  // Increased from 24.0 to 25.0
           glm::vec3(25.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
           "../textures/saturn.jpg", true},
          // Uranus
          {Uranus, 8.681e25f, 25362000.0f, 34.0f, 0.046f, 250.0f,
           0.0f,  // Increased from 30.0 to 34.0
           glm::vec3(34.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
           "../textures/uranus.jpg"},
          // Neptune
          {Neptune, 1.024e26f, 24622000.0f, 40.0f, 0.009f, 350.0f,
           0.0f,  // Increased from 36.0 to 40.0
           glm::vec3(40.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
           "../textures/neptune.jpg"}};
}

float CelestialBodyFactory::getRotationSpeed(const BodyType type) {
  switch (type) {
    case Sun:
      return 2.0f;
    case Mercury:
      return 10.0f;
    case Venus:
      return -5.0f;
    case Earth:
      return 15.0f;
    case Mars:
      return 14.0f;
    case Jupiter:
      return 45.0f;
    case Saturn:
      return 38.0f;
    case Uranus:
      return -20.0f;
    case Neptune:
      return 25.0f;
    default:
      return 0.0f;
  }
}

glm::vec3 CelestialBodyFactory::getScale(const BodyType type) {
  switch (type) {
    case Sun:
      return glm::vec3(3.0f);
    case Mercury:
      return glm::vec3(0.4f);
    case Venus:
      return glm::vec3(0.9f);
    case Earth:
      return glm::vec3(1.0f);
    case Mars:
      return glm::vec3(0.5f);
    case Jupiter:
      return glm::vec3(2.5f);
    case Saturn:
      return glm::vec3(2.2f);
    case Uranus:
      return glm::vec3(1.8f);
    case Neptune:
      return glm::vec3(1.7f);
    default:
      return glm::vec3(1.0f);
  }
}

glm::vec3 CelestialBodyFactory::getRotationAxis(const BodyType type) {
  if (type == Uranus) {
    return glm::vec3(0.8f, 0.2f, 0.1f);  // Tilted rotation
  }
  return glm::vec3(0.0f, 1.0f, 0.1f);  // Standard rotation
}

BodyInfo CelestialBodyFactory::getBodyInfo(const BodyType type) {
  switch (type) {
    case Sun:
      return {"SUN", 0.0f, 5505.0f, "STAR", 333000.0f, 1392700.0f, 0};
    case Mercury:
      return {"MERCURY", 0.39f, 167.0f, "TERRESTRIAL", 0.055f, 4879.0f, 0};
    case Venus:
      return {"VENUS", 0.72f, 464.0f, "TERRESTRIAL", 0.815f, 12104.0f, 0};
    case Earth:
      return {"EARTH", 1.0f, 15.0f, "TERRESTRIAL", 1.0f, 12742.0f, 1};
    case Mars:
      return {"MARS", 1.52f, -65.0f, "TERRESTRIAL", 0.107f, 6779.0f, 2};
    case Jupiter:
      return {"JUPITER", 5.20f, -110.0f, "GAS GIANT", 317.8f, 139820.0f, 79};
    case Saturn:
      return {"SATURN", 9.58f, -140.0f, "GAS GIANT", 95.2f, 116460.0f, 82};
    case Uranus:
      return {"URANUS", 19.22f, -195.0f, "ICE GIANT", 14.5f, 50724.0f, 27};
    case Neptune:
      return {"NEPTUNE", 30.05f, -200.0f, "ICE GIANT", 17.1f, 49244.0f, 14};
    default:
      return {"UNKNOWN", 0.0f, 0.0f, "UNKNOWN", 0.0f, 0.0f, 0};
  }
}

BodyProps CelestialBodyFactory::getBodyProps(const BodyType type) {
  for (auto& body : celestialBodies_) {
    if (body->getBodyProps().type == type) {
      return body->getBodyProps();
    }
  }
  return {};
}