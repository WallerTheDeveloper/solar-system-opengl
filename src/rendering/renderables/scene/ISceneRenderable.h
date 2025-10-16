  //
// Created by Daniel on 05-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_IRENDERABLE_H
#define SOLAR_SYSTEM_OPENGL_IRENDERABLE_H

#include "glm/detail/type_mat.hpp"

class ISceneRenderable {
 public:
  virtual ~ISceneRenderable() = default;
  virtual void render(glm::mat4 model, glm::mat4 view,
                      glm::mat4 projection) const = 0;
  virtual void update(float deltaTime) {};
};

#endif  // SOLAR_SYSTEM_OPENGL_IRENDERABLE_H
