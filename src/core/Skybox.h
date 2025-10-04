#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>

#include "../graphics/buffer/BufferManager.h"
#include "../graphics/buffer/BufferHandle.h"
#include "Engine.h"
#include "Shader.h"

class Skybox {
 public:
  Skybox(Engine* engine, std::vector<std::string> facesTextures);
  ~Skybox() = default;

  bool initialize(BufferManager* bufferManager);

  void render(const glm::mat4& view, const glm::mat4& projection);

 private:
  BufferHandle bufferHandle_;
  Engine* engine;

  std::vector<std::string> m_faces;

  unsigned int m_textureID;

  std::unique_ptr<Shader> m_shader;

  unsigned int m_indexCount;

  bool m_initialized;
  bool m_enabled;
};

#endif