//
// Created by Daniel on 25-Sep-25.
//

#include "Ring.h"

#include <core/Shader.h>

#include <graphics/buffer/BufferManager.h>

#include <vector>

Ring::Ring(BufferManager& bufferManager, TextureManager& textureManager)
    : bufferManager_(bufferManager),
      textureManager_(textureManager),
      textureID(0) {
  try {
    std::vector ringVertices = {
      // Positions (x, y, z)    // Texture coords (u, v)
      -3.0f, 0.0f, -3.0f, 0.0f, 0.0f,  // Bottom left
      3.0f,  0.0f, -3.0f, 1.0f, 0.0f,  // Bottom right
      3.0f,  0.0f, 3.0f,  1.0f, 1.0f,  // Top right
      -3.0f, 0.0f, 3.0f,  0.0f, 1.0f   // Top left
  };

    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

    std::vector<VertexAttribute> attributes = {
      // position
      {0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0},
      // texCoord
      {1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
       (void*)(3 * sizeof(float))}};

    bufferHandle = bufferManager_.createBufferSet("Saturn_Ring", ringVertices,
                                                  indices, attributes);

    textureID = textureManager_.createTexture("../textures/saturn_ring.png", GL_TEXTURE_2D,
                                              GL_REPEAT, GL_LINEAR);

    shader = std::make_unique<Shader>("../shaders/ring.vert",
                                      "../shaders/ring.frag");
  } catch (std::exception& e) {
    std::cout << "Failed to create ring: " << e.what() << std::endl;
  }
}

void Ring::render(const glm::mat4 model, const glm::mat4 view,
                  const glm::mat4 projection) const {
  bool cullFaceWasEnabled = glIsEnabled(GL_CULL_FACE);
  if (cullFaceWasEnabled) {
    glDisable(GL_CULL_FACE);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader->use();
  shader->setMat4("model", model);
  shader->setMat4("view", view);
  shader->setMat4("projection", projection);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  shader->setInt("ringTexture", 0);

  glBindVertexArray(bufferHandle.getVAO());
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  if (cullFaceWasEnabled) {
    glEnable(GL_CULL_FACE);
  }
  glDisable(GL_BLEND);
}