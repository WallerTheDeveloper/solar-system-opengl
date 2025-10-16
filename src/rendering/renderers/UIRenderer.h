//
// Created by Daniel on 30-Sep-25.
//

#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <rendering/renderers/TextRenderer.h>

struct BodyInfo;
struct RenderContext;

class UIRenderer {
 public:
  UIRenderer(TextRenderer& textRenderer);
  void render(const RenderContext& renderContext) const;

 private:
  TextRenderer& textRenderer_;

  void renderFPS(const RenderContext& renderContext) const;
  void renderControls() const;
  void renderTitle(const RenderContext& renderContext) const;
  void renderCameraPosition(const RenderContext& renderContext) const;
  void renderCrosshair(const RenderContext& renderContext) const;
  void renderPanel(const RenderContext& renderContext) const;
  // void renderBackground(float x, float y, float width, float height);

  // void renderCelestialBodyInfo(const glm::vec3& bodyPosition,
  //                              const CelestialBodyInfoPanel& bodyInfoPanel,
  //                              const CelestialBodyInfo& info,
  //                              const RenderContext& renderContext) const;
};

#endif  // UI_RENDERER_H