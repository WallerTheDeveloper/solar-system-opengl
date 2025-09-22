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
  unique_ptr<SphereMeshData> m_sphereMeshData;

  unsigned int m_indexCount;

  bool m_initialized;
  bool m_enabled;

  static constexpr float SKYBOX_RADIUS = 1000.0f;
  static constexpr unsigned int SKYBOX_SECTORS = 36;
  static constexpr unsigned int SKYBOX_STACKS = 18;
};

#endif