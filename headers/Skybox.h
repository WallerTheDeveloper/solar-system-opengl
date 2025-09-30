#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>

#include "Engine.h"
#include "Shader.h"

class Skybox {
 public:
  Skybox(Engine* engine, std::vector<std::string> facesTextures);
  ~Skybox();

  bool initialize();

  void render(const glm::mat4& view, const glm::mat4& projection);

 private:
  Engine* engine;

  std::vector<std::string> m_faces;

  unsigned int m_VAO, m_VBO, m_EBO;
  unsigned int m_textureID;

  std::unique_ptr<Shader> m_shader;
  std::unique_ptr<BoxMeshData> m_boxMeshData;

  unsigned int m_indexCount;

  bool m_initialized;
  bool m_enabled;
};

#endif