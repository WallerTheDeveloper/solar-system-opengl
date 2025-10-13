//
// Created by Daniel on 30-Sep-25.
//

#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <rendering/renderers/TextRenderer.h>
#include <rendering/renderables/ui/CelestialBodyInfoPanel.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct RenderContext;
class CelestialBody;

class UIRenderer {
 public:
  UIRenderer(TextRenderer& textRenderer);
  void render(const RenderContext& renderContext) const;

 private:
  TextRenderer& textRenderer_;

  bool showCelestialBodyInfo_ = false;

  void renderFPS(const RenderContext& renderContext) const;
  void renderControls() const;
  void renderTitle(const RenderContext& renderContext) const;
  void renderCameraPosition(const RenderContext& renderContext) const;
  void renderCrosshair(const RenderContext& renderContext) const;
  void renderCelestialBodyInfo(const glm::vec3& bodyPosition,
                               CelestialBodyInfoPanel& bodyInfoPanel,
                               const CelestialBodyInfo& info,
                               const RenderContext& renderContext) const;
};

#endif  // UI_RENDERER_H