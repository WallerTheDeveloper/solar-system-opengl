//
// Created by Daniel on 30-Sep-25.
//

#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include <vector>

#include "PlanetFactory.h"
#include "camera.h"
#include "planet.h"
#include "ring.h"
#include "skybox.h"

struct RenderContext {
  const Camera& camera;
  unsigned int screenWidth;
  unsigned int screenHeight;
  float currentTime;
};

class SceneRenderer {
 public:
  SceneRenderer(Skybox* skybox, Ring* saturnRing);

  void renderScene(const std::vector<Planet>& planets,
                   const RenderContext& context);

 private:
  Skybox* skybox_;
  Ring* saturnRing_;

  glm::mat4 calculateViewMatrix(const Camera& camera) const;
  glm::mat4 calculateProjectionMatrix(const RenderContext& context) const;
  glm::mat4 calculateModelMatrix(const Planet& planet, float currentTime) const;
};

#endif  // SCENE_RENDERER_H