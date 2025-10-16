//
// Created by Daniel on 15-Oct-25.
//

#include "RenderHelper.h"

#include "glm/detail/type_vec.hpp"
#include <rendering/RenderContext.h>

ScreenPosition RenderHelper::worldToScreen(
    const glm::vec3& worldPos, const RenderContext& renderContext) {
  ScreenPosition result{};

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