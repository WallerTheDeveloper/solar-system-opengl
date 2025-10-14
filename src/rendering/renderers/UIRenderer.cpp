//
// Created by Daniel on 30-Sep-25.
//

#include "UIRenderer.h"

#include <rendering/RenderContext.h>

#include <rendering/renderables/ui/CelestialBodyInfoPanel.h>

UIRenderer::UIRenderer(TextRenderer& textRenderer)
    : textRenderer_(textRenderer){}

void UIRenderer::render(const RenderContext& renderContext) const {
  renderFPS(renderContext);
  renderControls();
  renderTitle(renderContext);
  renderCameraPosition(renderContext);
  renderCrosshair(renderContext);
  // Render planet info if selected
  // if (selectedPlanetIndex_ >= 0 &&
  //     selectedPlanetIndex_ < static_cast<int>(celestialBodies_.size())) {
  //   const auto& selectedPlanet = celestialBodies_[selectedPlanetIndex_];
  //   CelestialBodyInfo info =
  //       CelestialBodyInfoPanel::getBodyInfo(selectedPlanet->type);
  //
  //   renderCelestialBodyInfo(selectedPlanet->position, info, renderContext);
  //     }
}

void UIRenderer::renderFPS(const RenderContext& renderContext) const {
  float fpsX = renderContext.screenWidth - 500.0f;
  float fpsY = 20.0f;

  std::string fpsText = "FPS: " + std::to_string(static_cast<int>(renderContext.fps));
  textRenderer_.renderText(fpsText, fpsX, fpsY, 3.0f,
                            glm::vec3(0.0f, 1.0f, 0.0f));

  if (renderContext.fps > 0.0f) {
    float frameTime = 1000.0f / renderContext.fps;
    std::string frameTimeText =
        std::to_string(static_cast<int>(frameTime)) + " MS";
    textRenderer_.renderText(frameTimeText, fpsX, fpsY + 30.0f, 2.5f,
                              glm::vec3(0.8f, 0.8f, 0.8f));
  }
}

void UIRenderer::renderControls() const {
  textRenderer_.renderText("CONTROLS", 20.0f, 20.0f, 2.5f,
                          glm::vec3(1.0f, 1.0f, 0.0f));
  textRenderer_.renderText("W,A,S,D      - MOVE", 20.0f, 50.0f, 2.2f,
                          glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer_.renderText("E            - MOVE UP", 20.0f, 80.0f, 2.2f,
                          glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer_.renderText("Q            - MOVE DOWN", 20.0f, 110.0f, 2.2f,
                          glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer_.renderText("SHIFT        - SPRINT", 20.0f, 140.0f, 2.2f,
                          glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer_.renderText("MOUSE        - LOOK AROUND", 20.0f, 170.0f, 2.2f,
                          glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer_.renderText("SCROLL       - ZOOM", 20.0f, 200.0f, 2.2f,
                          glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer_.renderText("SHIFT+ENTER  - FULL SCREEN", 20.0f, 230.0f, 2.2f,
                          glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer_.renderText("CLICK        - SELECT PLANET", 20.0f, 260.0f, 2.2f,
                          glm::vec3(0.5f, 1.0f, 0.5f));
  textRenderer_.renderText("ESC          - EXIT", 20.0f, 290.0f, 2.2f,
                            glm::vec3(1.0f, 0.5f, 0.5f));
}

void UIRenderer::renderTitle(const RenderContext& renderContext) const {
  textRenderer_.renderText(
      "SOLAR SYSTEM SIMULATION", (renderContext.screenWidth / 2.0f) - 300.0f,
      (renderContext.screenHeight / 15.0f), 3.0f, glm::vec3(0.0f, 0.8f, 1.0f));
}

void UIRenderer::renderCameraPosition(const RenderContext& renderContext) const {
  glm::vec3 camPos = renderContext.camera.Position;
  std::string posText =
      "POSITION: X:" + std::to_string(static_cast<int>(camPos.x)) +
      " Y:" + std::to_string(static_cast<int>(camPos.y)) +
      " Z:" + std::to_string(static_cast<int>(camPos.z));

  textRenderer_.renderText(posText, (renderContext.screenWidth / 2.0f) - 300.0f,
                            (renderContext.screenHeight / 8.0f), 3.0f,
                            glm::vec3(0.0f, 0.8f, 1.0f));
}

void UIRenderer::renderCrosshair(const RenderContext& renderContext) const {
  float centerX = renderContext.screenWidth / 2.0f;
  float centerY = renderContext.screenHeight / 2.0f;
  textRenderer_.renderText(":+:", centerX - 12.0f, centerY - 12.0f, 4.0f,
                            glm::vec3(0.0f, 1.0f, 0.0f));
}

void UIRenderer::renderCelestialBodyInfo(
    const glm::vec3& bodyPosition, const CelestialBodyInfoPanel& bodyInfoPanel,
    const CelestialBodyInfo& info, const RenderContext& renderContext) const {
  bodyInfoPanel.renderPanel(bodyPosition, info, renderContext);
}