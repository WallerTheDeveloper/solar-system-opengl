//
// Created by Daniel on 29-Sep-25.
//

#include "CelestialBodyPicker.h"

#include <celestialbody/CelestialBodyFactory.h>
#include <core/Camera.h>
#include <helpers/RayIntersection.h>
#include <rendering/renderables/scene/CelestialBody.h>

#include <iostream>
#include <limits>

#include "glm/detail/type_vec3.hpp"
#include "glm/gtc/packing.inl"

CelestialBodyPicker::SelectionResult CelestialBodyPicker::pickBody(
    const Camera& camera,
    const std::function<void(int)>& onBodyPicked) {


  SelectionResult result{false, 0, std::numeric_limits<float>::max()};
  auto& celestialBodies = CelestialBodyFactory::getCelestialBodies();

  glm::vec3 rayOrigin = camera.Position;
  glm::vec3 rayDirection = camera.getRayDirection();

  std::cout << "Ray Origin: (" << rayOrigin.x << ", " << rayOrigin.y << ", "
            << rayOrigin.z << ")" << std::endl;
  std::cout << "Ray Direction: (" << rayDirection.x << ", " << rayDirection.y
            << ", " << rayDirection.z << ")" << std::endl;

  // std::vector<glm::vec3> scales;
  // scales.reserve(celestialBodies.size());
  //
  // for (const auto& body : celestialBodies) {
  //   scales.push_back(CelestialBodyFactory::getScale(body->getBodyProps().type));
  // }

  for (size_t i = 0; i < celestialBodies.size(); i++) {
    // float sphereRadius = scales[i].x;
    float sphereRadius = CelestialBodyFactory::getScale(celestialBodies[i]->getBodyProps().type).x;
    float distance;

    if (RayIntersection::raySphereIntersection(
            rayOrigin, rayDirection,
            celestialBodies[i]->getBodyProps().position, sphereRadius,
            distance)) {
      std::cout << "  Hit planet " << i << " at distance " << distance
                << " (radius: " << sphereRadius << ")" << std::endl;

      if (distance < result.distance) {
        result.hit = true;
        result.planetIndex = i;
        result.distance = distance;
      }
    }
  }

  int selectedPlanetIndex = -1;
  if (result.hit) {
    std::cout << "Selected body: "
              << CelestialBodyFactory::getBodyInfo(
                     celestialBodies[result.planetIndex]->getBodyProps().type)
                     .name
              << std::endl;
    selectedPlanetIndex = static_cast<int>(result.planetIndex);

    if (selectedPlanetIndex >= 0 &&
        selectedPlanetIndex < static_cast<int>(celestialBodies.size())) {
    }
  } else {
    selectedPlanetIndex = -1;
    std::cout << "No celestial body selected" << std::endl;
  }

  onBodyPicked(selectedPlanetIndex);
  return result;
}