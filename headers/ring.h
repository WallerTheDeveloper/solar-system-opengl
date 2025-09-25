//
// Created by Daniel on 25-Sep-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_RING_H
#define SOLAR_SYSTEM_OPENGL_RING_H
#include "engine.h"
#include "shader.h"
#include "debug_utils.h"

class Ring {
 public:
  void create(Engine* engine, const char* ringTexturePath);
  void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

 private:
  unsigned int VAO, VBO, EBO;
  unsigned int textureID;
  std::unique_ptr<Shader> shader;
};

#endif  // SOLAR_SYSTEM_OPENGL_RING_H
