//
// Created by Daniel on 05-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_RENDERCONTEXT_H
#define SOLAR_SYSTEM_OPENGL_RENDERCONTEXT_H

#include <core/Camera.h>
#include <CelestialBodyTypes.h>

struct RenderContext {
  const Camera& camera;
  const BodyType& selectedBodyType;
  unsigned int screenWidth;
  unsigned int screenHeight;
  float currentTime;
  float fps;
  bool canRenderPanel;
};

#endif  // SOLAR_SYSTEM_OPENGL_RENDERCONTEXT_H
