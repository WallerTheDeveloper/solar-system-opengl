//
// Created by Daniel on 30-Sep-25.
//

#include "UIRenderer.h"

#include <CelestialBodyTypes.h>
#include <helpers/RenderHelper.h>
#include <rendering/RenderContext.h>
#include <rendering/ScreenPosition.h>
#include <celestialbody/CelestialBodyFactory.h>

#include <iomanip>
#include <ios>
#include <sstream>


UIRenderer::UIRenderer(TextRenderer& textRenderer)
    : textRenderer_(textRenderer) {}

void UIRenderer::render(const RenderContext& renderContext) const {
  renderFPS(renderContext);
  renderControls();
  renderTitle(renderContext);
  renderCameraPosition(renderContext);
  renderCrosshair(renderContext);
  renderPanel(renderContext);
}

void UIRenderer::renderFPS(const RenderContext& renderContext) const {
  float fpsX = renderContext.screenWidth - 500.0f;
  float fpsY = 20.0f;

  std::string fpsText =
      "FPS: " + std::to_string(static_cast<int>(renderContext.fps));
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

void UIRenderer::renderCameraPosition(
    const RenderContext& renderContext) const {
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
  textRenderer_.renderText(":+:", centerX - 12.0f, centerY - 12.0f, 3.0f,
                           glm::vec3(0.0f, 1.0f, 0.0f));
}

void UIRenderer::renderPanel(const RenderContext& renderContext) const {
  if (!renderContext.canRenderPanel) {
    return;
  }
  // Calculate the position above the planet
  glm::vec3 panelWorldPos =
      CelestialBodyFactory::getBodyProps(Sun).position + glm::vec3(0.0f, 2.0f, 0.0f);

  ScreenPosition screenPos =
      RenderHelper::worldToScreen(panelWorldPos, renderContext);

  if (!screenPos.visible) {
    return;
  }

  // Panel dimensions
  const float panelWidth = 350.0f;
  const float panelHeight = 200.0f;
  const float lineHeight = 25.0f;
  const float textScale = 2.2f;
  const float titleScale = 3.0f;

  // Center panel horizontally
  float panelX = screenPos.x - panelWidth / 2.0f;
  float panelY =
      screenPos.y - panelHeight - 30.0f;  // Position above the planet

  // Clamp to screen bounds
  if (panelX < 10.0f) panelX = 10.0f;
  if (panelX + panelWidth > renderContext.screenWidth - 10.0f)
    panelX = renderContext.screenWidth - panelWidth - 10.0f;
  if (panelY < 10.0f) panelY = 10.0f;

  // Render title
  textRenderer_.renderText(CelestialBodyFactory::getBodyInfo(Sun).name, panelX + 10.0f,
                           panelY + 10.0f, titleScale,
                           glm::vec3(1.0f, 0.9f, 0.2f));

  float currentY = panelY + 45.0f;

  // Render type with color coding
  glm::vec3 typeColor = glm::vec3(0.5f, 0.9f, 1.0f);
  if (CelestialBodyFactory::getBodyInfo(Sun).type == "STAR")
    typeColor = glm::vec3(1.0f, 0.9f, 0.3f);
  else if (CelestialBodyFactory::getBodyInfo(Sun).type == "GAS GIANT")
    typeColor = glm::vec3(0.9f, 0.7f, 0.5f);
  else if (CelestialBodyFactory::getBodyInfo(Sun).type == "ICE GIANT")
    typeColor = glm::vec3(0.6f, 0.8f, 1.0f);

  textRenderer_.renderText("TYPE: " + CelestialBodyFactory::getBodyInfo(Sun).type,
                           panelX + 10.0f, currentY, textScale, typeColor);
  currentY += lineHeight;

  // Distance from Sun
  std::stringstream distStream;
  distStream << std::fixed << std::setprecision(2)
             << CelestialBodyFactory::getBodyInfo(Sun).distanceFromSun;
  textRenderer_.renderText("DISTANCE: " + distStream.str() + " AU",
                           panelX + 10.0f, currentY, textScale,
                           glm::vec3(0.8f, 0.8f, 1.0f));
  currentY += lineHeight;

  // Temperature
  std::stringstream tempStream;
  tempStream << std::fixed << std::setprecision(0)
             << CelestialBodyFactory::getBodyInfo(Sun).temperature;
  textRenderer_.renderText("TEMP: " + tempStream.str() + " C", panelX + 10.0f,
                           currentY, textScale, glm::vec3(1.0f, 0.6f, 0.4f));
  currentY += lineHeight;

  // Mass
  std::stringstream massStream;
  massStream << std::fixed << std::setprecision(2)
             << CelestialBodyFactory::getBodyInfo(Sun).mass;
  textRenderer_.renderText("MASS: " + massStream.str() + " EARTHS",
                           panelX + 10.0f, currentY, textScale,
                           glm::vec3(0.7f, 1.0f, 0.7f));
  currentY += lineHeight;

  // Diameter
  std::stringstream diamStream;
  diamStream << std::fixed << std::setprecision(0)
             << CelestialBodyFactory::getBodyInfo(Sun).diameter;
  textRenderer_.renderText("DIAMETER: " + diamStream.str() + " KM",
                           panelX + 10.0f, currentY, textScale,
                           glm::vec3(0.9f, 0.9f, 0.9f));
  currentY += lineHeight;

  // Moons
  textRenderer_.renderText(
      "MOONS: " + std::to_string(CelestialBodyFactory::getBodyInfo(Sun).moons), panelX + 10.0f,
      currentY, textScale, glm::vec3(0.8f, 0.8f, 0.9f));
}

// void CelestialBodyInfoPanel::renderBackground(float x, float y, float width,
// float height) {
//     // We'll render a semi-transparent background using lines
//     // since we don't have a quad renderer, we'll use text characters as
//     blocks
//
//     const float lineHeight = 8.0f;
//     int numLines = static_cast<int>(height / lineHeight);
//
//     for (int i = 0; i < numLines; i++) {
//         std::string line(static_cast<int>(width / 8.0f), ' ');
//         textRenderer->renderText(line, x, y + i * lineHeight, 1.0f,
//                                glm::vec3(0.1f, 0.1f, 0.15f));
//     }
// }