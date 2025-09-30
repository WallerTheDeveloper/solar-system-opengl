//
// Created by Daniel on 30-Sep-25.
//

#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include <vector>

#include "../Ring.h"
#include "../celestialbody/CelestialBody.h"
#include "../celestialbody/CelestialBodyFactory.h"
#include "../core/Camera.h"
#include "../core/Skybox.h"

struct RenderContext {
  const Camera& camera;
  unsigned int screenWidth;
  unsigned int screenHeight;
  float currentTime;
};

class SceneRenderer {
 public:
  SceneRenderer(Skybox* skybox, Ring* saturnRing);

  void renderScene(const std::vector<std::unique_ptr<CelestialBody>>& celestialBodies,
                   const RenderContext& context);

 private:
  Skybox* skybox_;
  Ring* saturnRing_;

  glm::mat4 calculateViewMatrix(const Camera& camera) const;
  glm::mat4 calculateProjectionMatrix(const RenderContext& context) const;
  glm::mat4 calculateModelMatrix(const std::unique_ptr<CelestialBody>& celestialBody, float currentTime) const;
};

#endif  // SCENE_RENDERER_H