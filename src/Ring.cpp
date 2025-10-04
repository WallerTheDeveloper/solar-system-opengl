//
// Created by Daniel on 25-Sep-25.
//

#include "Ring.h"

void Ring::create(Engine* engine, BufferManager* bufferManager, const char* ringTexturePath) {
  std::vector<float> ringVertices = {
      // Positions (x, y, z)    // Texture coords (u, v)
      -3.0f, 0.0f, -3.0f,       0.0f, 0.0f,  // Bottom left
      3.0f,  0.0f, -3.0f,       1.0f, 0.0f,  // Bottom right
      3.0f,  0.0f, 3.0f,        1.0f, 1.0f,  // Top right
      -3.0f, 0.0f, 3.0f,        0.0f, 1.0f   // Top left
  };

  std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

  std::vector<VertexAttribute> attributes = {
      // position
      {0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0},
      // texCoord
      {1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))}
  };

  bufferHandle = bufferManager->createBufferSet(
        "Saturn_Ring",
        ringVertices,
        indices,
        attributes
    );

  textureID = engine->addTextureToObject(ringTexturePath, GL_TEXTURE_2D,
                                         GL_REPEAT, GL_LINEAR);

  shader = std::make_unique<Shader>
  ("../shaders/ring.vert", "../shaders/ring.frag");
}

void Ring::render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
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