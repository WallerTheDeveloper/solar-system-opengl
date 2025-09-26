//
// Created by Daniel on 25-Sep-25.
//

#include "ring.h"

void Ring::create(Engine* engine, const char* ringTexturePath) {
  float ringVertices[] = {
      // Positions (x, y, z)    // Texture coords (u, v)
      -3.0f, 0.0f, -3.0f,       0.0f, 0.0f,  // Bottom left
      3.0f,  0.0f, -3.0f,       1.0f, 0.0f,  // Bottom right
      3.0f,  0.0f, 3.0f,        1.0f, 1.0f,  // Top right
      -3.0f, 0.0f, 3.0f,        0.0f, 1.0f   // Top left
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  engine->generateVAO(&VAO);
  engine->bindVAO(VAO);

  engine->generateBuffer(&VBO);
  engine->bindBuffer(GL_ARRAY_BUFFER, VBO);
  engine->setBufferData(GL_ARRAY_BUFFER, sizeof(ringVertices), ringVertices,
                        GL_STATIC_DRAW);

  engine->generateBuffer(&EBO);
  engine->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  engine->setBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                        GL_STATIC_DRAW);

  engine->defineVertexLayout(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                             (void*)0);
  engine->defineVertexLayout(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                             (void*)(3 * sizeof(float)));

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

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  if (cullFaceWasEnabled) {
    glEnable(GL_CULL_FACE);
  }
  glDisable(GL_BLEND);
}