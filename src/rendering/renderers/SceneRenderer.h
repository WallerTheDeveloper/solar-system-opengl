//
// Created by Daniel on 30-Sep-25.
//

#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include <memory>
#include <queue>

#include "glm/detail/type_mat.hpp"

class CelestialBody;
class ISceneRenderable;

struct RenderContext;

class SceneRenderer {
 public:
  SceneRenderer();
  ~SceneRenderer();
  void render(const std::deque<ISceneRenderable*>& renderables,
                   const RenderContext& context) const;

 private:
  glm::mat4 calculateModelMatrix(const CelestialBody& body, float currentTime) const;
};

#endif  // SCENE_RENDERER_H