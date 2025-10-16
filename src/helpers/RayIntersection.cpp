//
// Created by Daniel on 15-Oct-25.
//

#include "RayIntersection.h"

#include "glm/detail/func_geometric.inl"

bool RayIntersection::raySphereIntersection(const glm::vec3& rayOrigin,
                                            const glm::vec3& rayDirection,
                                            const glm::vec3& sphereCenter,
                                            float sphereRadius,
                                            float& distance) {
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