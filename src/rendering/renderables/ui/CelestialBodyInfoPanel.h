//
// Created by Daniel on 29-Sep-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_PLANETINFOPANEL_H
#define SOLAR_SYSTEM_OPENGL_PLANETINFOPANEL_H
#include <CelestialBodyTypes.h>
#include <rendering/renderers/TextRenderer.h>

struct RenderContext;

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
  CelestialBodyInfoPanel(TextRenderer& textRenderer);

  void renderPanel(const glm::vec3& worldPosition, const CelestialBodyInfo& info,
                   const RenderContext& renderContext) const;

  static CelestialBodyInfo getBodyInfo( BodyType type);

 private:
  TextRenderer& textRenderer_;

  struct ScreenPosition {
    float x;
    float y;
    bool visible;
  };

  ScreenPosition worldToScreen(const glm::vec3& worldPos, const RenderContext& renderContext) const;

  // void renderBackground(float x, float y, float width, float height);
};

#endif  // SOLAR_SYSTEM_OPENGL_PLANETINFOPANEL_H
