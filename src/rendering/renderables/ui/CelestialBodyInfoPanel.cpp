//
// Created by Daniel on 29-Sep-25.
//
#include "CelestialBodyInfoPanel.h"

#include <rendering/RenderContext.h>

#include <glm/gtc/matrix_transform.hpp>
#include <iomanip>
#include <sstream>


CelestialBodyInfoPanel::CelestialBodyInfoPanel(TextRenderer& textRenderer)
    : textRenderer_(textRenderer) {}

CelestialBodyInfo CelestialBodyInfoPanel::getBodyInfo(BodyType type) {
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

CelestialBodyInfoPanel::ScreenPosition CelestialBodyInfoPanel::worldToScreen(
    const glm::vec3& worldPos,
    const RenderContext& renderContext) {

    ScreenPosition result;

    // Get view and projection matrices
    glm::mat4 view = renderContext.camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(renderContext.camera.Zoom),
        static_cast<float>(renderContext.screenWidth) / renderContext.screenHeight, 0.1f, 10000.0f);

    // Transform to clip space
    glm::vec4 clipSpace = projection * view * glm::vec4(worldPos, 1.0f);

    // Check if behind camera
    if (clipSpace.w <= 0.0f) {
        result.visible = false;
        return result;
    }

    // Perspective division to NDC
    glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;

    // Check if outside screen bounds
    if (ndc.x < -1.0f || ndc.x > 1.0f || ndc.y < -1.0f || ndc.y > 1.0f) {
        result.visible = false;
        return result;
    }

    // Convert to screen coordinates (OpenGL uses bottom-left origin, but we want top-left)
    result.x = (ndc.x + 1.0f) * 0.5f * renderContext.screenWidth;
    result.y = (1.0f - ndc.y) * 0.5f * renderContext.screenHeight;  // Flip Y for top-left origin
    result.visible = true;

    return result;
}

// void CelestialBodyInfoPanel::renderBackground(float x, float y, float width, float height) {
//     // We'll render a semi-transparent background using lines
//     // since we don't have a quad renderer, we'll use text characters as blocks
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

void CelestialBodyInfoPanel::renderPanel(const glm::vec3& worldPosition,
                                  const CelestialBodyInfo& info, const RenderContext& renderContext) {
  // Calculate the position above the planet
    glm::vec3 panelWorldPos = worldPosition + glm::vec3(0.0f, 2.0f, 0.0f);

    ScreenPosition screenPos = worldToScreen(panelWorldPos, renderContext);

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
    float panelY = screenPos.y - panelHeight - 30.0f;  // Position above the planet

    // Clamp to screen bounds
    if (panelX < 10.0f) panelX = 10.0f;
    if (panelX + panelWidth > renderContext.screenWidth - 10.0f) panelX = renderContext.screenWidth - panelWidth - 10.0f;
    if (panelY < 10.0f) panelY = 10.0f;

    // Render background (optional - we'll skip for now since we don't have quad rendering)

    // Render title
    textRenderer_.renderText(info.name, panelX + 10.0f, panelY + 10.0f, titleScale,
                           glm::vec3(1.0f, 0.9f, 0.2f));

    float currentY = panelY + 45.0f;

    // Render type with color coding
    glm::vec3 typeColor = glm::vec3(0.5f, 0.9f, 1.0f);
    if (info.type == "STAR") typeColor = glm::vec3(1.0f, 0.9f, 0.3f);
    else if (info.type == "GAS GIANT") typeColor = glm::vec3(0.9f, 0.7f, 0.5f);
    else if (info.type == "ICE GIANT") typeColor = glm::vec3(0.6f, 0.8f, 1.0f);

    textRenderer_.renderText("TYPE: " + info.type, panelX + 10.0f, currentY, textScale,
                           typeColor);
    currentY += lineHeight;

    // Distance from Sun
    std::stringstream distStream;
    distStream << std::fixed << std::setprecision(2) << info.distanceFromSun;
    textRenderer_.renderText("DISTANCE: " + distStream.str() + " AU",
                           panelX + 10.0f, currentY, textScale,
                           glm::vec3(0.8f, 0.8f, 1.0f));
    currentY += lineHeight;

    // Temperature
    std::stringstream tempStream;
    tempStream << std::fixed << std::setprecision(0) << info.temperature;
    textRenderer_.renderText("TEMP: " + tempStream.str() + " C",
                           panelX + 10.0f, currentY, textScale,
                           glm::vec3(1.0f, 0.6f, 0.4f));
    currentY += lineHeight;

    // Mass
    std::stringstream massStream;
    massStream << std::fixed << std::setprecision(2) << info.mass;
    textRenderer_.renderText("MASS: " + massStream.str() + " EARTHS",
                           panelX + 10.0f, currentY, textScale,
                           glm::vec3(0.7f, 1.0f, 0.7f));
    currentY += lineHeight;

    // Diameter
    std::stringstream diamStream;
    diamStream << std::fixed << std::setprecision(0) << info.diameter;
    textRenderer_.renderText("DIAMETER: " + diamStream.str() + " KM",
                           panelX + 10.0f, currentY, textScale,
                           glm::vec3(0.9f, 0.9f, 0.9f));
    currentY += lineHeight;

    // Moons
    textRenderer_.renderText("MOONS: " + std::to_string(info.moons),
                           panelX + 10.0f, currentY, textScale,
                           glm::vec3(0.8f, 0.8f, 0.9f));
}