#ifndef SKYBOX_H
#define SKYBOX_H

#include <rendering/renderables/scene/ISceneRenderable.h>

#include <glm/glm.hpp>
#include <memory>

#include <core/texturing/TextureManager.h>
#include <graphics/buffer/BufferHandle.h>

class Shader;

class Skybox : public ISceneRenderable {
 public:
  Skybox(BufferManager& bufferManager, TextureManager& textureManager);
  ~Skybox() override = default;

  void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const override;

 private:
  BufferManager& bufferManager_;
  BufferHandle bufferHandle_;
  TextureManager& textureManager_;

  unsigned int m_textureID;

  std::unique_ptr<Shader> m_shader;

  unsigned int m_indexCount;

  bool m_enabled;
};

#endif