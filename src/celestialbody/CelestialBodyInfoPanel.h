//
// Created by Daniel on 29-Sep-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_PLANETINFOPANEL_H
#define SOLAR_SYSTEM_OPENGL_PLANETINFOPANEL_H

#include <glm/glm.hpp>
#include <string>

#include "../core/Camera.h"
#include "../renderers/TextRenderer.h"
#include "CelestialBody.h"

struct CelestialBodyInfo {
  std::string name;
  float distanceFromSun;  // in AU
  float temperature;      // in Celsius
  std::string type;
  float mass;      // relative to Earth
  float diameter;  // in km
  int moons;
};

class CelestialBodyInfoPanel {
 public:
  CelestialBodyInfoPanel(TextRenderer* textRenderer);

  void renderPanel(const glm::vec3& worldPosition, const CelestialBodyInfo& info,
                   const Camera& camera, float screenWidth, float screenHeight);

  static CelestialBodyInfo getBodyInfo(CelestialBody::BodyType type);

 private:
  TextRenderer* textRenderer;

  struct ScreenPosition {
    float x;
    float y;
    bool visible;
  };

  ScreenPosition worldToScreen(const glm::vec3& worldPos, const Camera& camera,
                               float screenWidth, float screenHeight);

  void renderBackground(float x, float y, float width, float height);
  void renderBorder(float x, float y, float width, float height);
};

#endif  // SOLAR_SYSTEM_OPENGL_PLANETINFOPANEL_H
