//
// Created by Daniel on 15-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_RENDERHELPER_H
#define SOLAR_SYSTEM_OPENGL_RENDERHELPER_H
#include <rendering/ScreenPosition.h>

#include "glm/detail/type_vec.hpp"

struct RenderContext;

class RenderHelper {
public:
  static ScreenPosition worldToScreen(const glm::vec3& worldPos, const RenderContext& renderContext);
};

#endif  // SOLAR_SYSTEM_OPENGL_RENDERHELPER_H
