//
// Created by Daniel on 05-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_RENDERCONTEXT_H
#define SOLAR_SYSTEM_OPENGL_RENDERCONTEXT_H

#include <core/Camera.h>

struct RenderContext {
  const Camera& camera;
  unsigned int screenWidth;
  unsigned int screenHeight;
  float currentTime;
  float fps;
};

#endif  // SOLAR_SYSTEM_OPENGL_RENDERCONTEXT_H
