//
// Created by Daniel on 29-Sep-25.
//

#include <limits>

#include "CelestialBodyPicker.h"

bool CelestialBodyPicker::raySphereIntersection(const glm::vec3& rayOrigin,
                                         const glm::vec3& rayDirection,
                                         const glm::vec3& sphereCenter,
                                         float sphereRadius, float& distance) {
  glm::vec3 oc = rayOrigin - sphereCenter;
  float a = glm::dot(rayDirection, rayDirection);
  float b = 2.0f * glm::dot(oc, rayDirection);
  float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
  float discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    return false;
  }

  float t = (-b - sqrt(discriminant)) / (2.0f * a);
  if (t < 0) {
    t = (-b + sqrt(discriminant)) / (2.0f * a);
  }

  if (t < 0) {
    return false;
  }

  distance = t;
  return true;
}

CelestialBodyPicker::SelectionResult CelestialBodyPicker::pickPlanet(
    const Camera& camera, const std::vector<std::unique_ptr<CelestialBody>>& celestialBodies,
    const std::vector<glm::vec3>& scales) {
  SelectionResult result{false, 0, std::numeric_limits<float>::max()};
  glm::vec3 rayOrigin = camera.Position;
  glm::vec3 rayDirection = camera.getRayDirection();

  std::cout << "Ray Origin: (" << rayOrigin.x << ", " << rayOrigin.y << ", "
            << rayOrigin.z << ")" << std::endl;
  std::cout << "Ray Direction: (" << rayDirection.x << ", " << rayDirection.y
            << ", " << rayDirection.z << ")" << std::endl;

  for (size_t i = 0; i < celestialBodies.size(); i++) {
    float sphereRadius = scales[i].x;
    float distance;

    if (raySphereIntersection(rayOrigin, rayDirection, celestialBodies[i]->position,
                              sphereRadius, distance)) {
      std::cout << "  Hit planet " << i << " at distance " << distance
                << " (radius: " << sphereRadius << ")" << std::endl;

      if (distance < result.distance) {
        result.hit = true;
        result.planetIndex = i;
        result.distance = distance;
      }
    }
  }

  if (result.hit) {
    std::cout << "Selected celestial body " << result.planetIndex << std::endl;
  } else {
    std::cout << "No celestial body selected" << std::endl;
  }

  return result;
}