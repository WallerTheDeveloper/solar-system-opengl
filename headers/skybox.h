#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>

#include "engine.h"
#include "shader.h"

class Skybox {
 public:
  Skybox(Engine* engine, const string& texturePath);
  ~Skybox();

  bool create();

  void render(const glm::mat4& view, const glm::mat4& projection);

 private:
  Engine* engine;

  const string m_texturePath;

  unsigned int m_VAO, m_VBO, m_EBO;
  unsigned int m_textureID;

  unique_ptr<Shader> m_shader;
  unique_ptr<BoxMeshData> m_boxMeshData;

  unsigned int m_indexCount;

  bool m_initialized;
  bool m_enabled;

  static constexpr float SKYBOX_WIDTH = 2000.0f;
  static constexpr float SKYBOX_HEIGHT = 2000.0f;
  static constexpr float SKYBOX_DEPTH = 2000.0f;
};

#endif