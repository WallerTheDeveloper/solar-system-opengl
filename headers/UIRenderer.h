//
// Created by Daniel on 30-Sep-25.
//

#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <glm/glm.hpp>
#include <string>

#include "Camera.h"
#include "PlanetInfoPanel.h"
#include "TextRenderer.h"

class UIRenderer {
 public:
  UIRenderer(TextRenderer* textRenderer, CelestialBodyInfoPanel* planetInfoPanel);

  void renderUI(float fps, const Camera& camera, unsigned int screenWidth,
                unsigned int screenHeight);

  void renderCelestialBodyInfo(const glm::vec3& bodyPosition, const CelestialBodyInfo& info,
                        const Camera& camera, unsigned int screenWidth,
                        unsigned int screenHeight);

  void setShowCelestialBodyInfo(bool show) { showCelestialBodyInfo_ = show; }

 private:
  TextRenderer* textRenderer_;
  CelestialBodyInfoPanel* planetInfoPanel_;
  bool showCelestialBodyInfo_ = false;

  void renderFPS(float fps, unsigned int screenWidth);
  void renderControls();
  void renderTitle(unsigned int screenWidth, unsigned int screenHeight);
  void renderCameraPosition(const Camera& camera, unsigned int screenWidth,
                            unsigned int screenHeight);
  void renderCrosshair(unsigned int screenWidth, unsigned int screenHeight);
};

#endif  // UI_RENDERER_H