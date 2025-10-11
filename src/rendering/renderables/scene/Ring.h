//
// Created by Daniel on 25-Sep-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_RING_H
#define SOLAR_SYSTEM_OPENGL_RING_H

#include <rendering/renderables/scene/ISceneRenderable.h>

#include <memory>

#include <core/texturing/TextureManager.h>
#include <graphics/buffer/BufferHandle.h>

class Shader;

class Ring : public ISceneRenderable {
 public:
  Ring(BufferManager& bufferManager, TextureManager& textureManager);
  ~Ring() override;
  void create(const char* texturePath);
  void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const override;

private:
  BufferManager& bufferManager_;
  BufferHandle bufferHandle;
  TextureManager& textureManager_;

  unsigned int textureID;
  std::unique_ptr<Shader> shader;
};

#endif  // SOLAR_SYSTEM_OPENGL_RING_H
