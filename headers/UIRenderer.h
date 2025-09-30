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
  UIRenderer(TextRenderer* textRenderer, PlanetInfoPanel* planetInfoPanel);

  void renderUI(float fps, const Camera& camera, unsigned int screenWidth,
                unsigned int screenHeight);

  void renderPlanetInfo(const glm::vec3& planetPosition, const PlanetInfo& info,
                        const Camera& camera, unsigned int screenWidth,
                        unsigned int screenHeight);

  void setShowPlanetInfo(bool show) { showPlanetInfo_ = show; }

 private:
  TextRenderer* textRenderer_;
  PlanetInfoPanel* planetInfoPanel_;
  bool showPlanetInfo_ = false;

  void renderFPS(float fps, unsigned int screenWidth);
  void renderControls();
  void renderTitle(unsigned int screenWidth, unsigned int screenHeight);
  void renderCameraPosition(const Camera& camera, unsigned int screenWidth,
                            unsigned int screenHeight);
  void renderCrosshair(unsigned int screenWidth, unsigned int screenHeight);
};

#endif  // UI_RENDERER_H