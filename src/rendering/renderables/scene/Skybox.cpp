#include "Skybox.h"

#include <core/Shader.h>

#include <iostream>

#include <graphics/buffer/BufferManager.h>

Skybox::Skybox(BufferManager& bufferManager, TextureManager& textureManager)
    : textureManager_(textureManager),
      bufferManager_(bufferManager),
      m_textureID(0),
      m_indexCount(0),
      m_initialized(false),
      m_enabled(true) {}

void Skybox::create(const std::vector<std::string>& facesTextures) {
  if (m_initialized) {
    std::cerr << "Skybox already initialized! Check if you calling Skybox::create more that once!" << std::endl;
    return;
  }

  std::cout << "\n=== SKYBOX CREATION ===" << std::endl;

  try {
    m_shader = std::make_unique<Shader>("../shaders/skybox.vert",
                                        "../shaders/skybox.frag");

    if (m_shader == nullptr) {
      std::cerr << "SKYBOX CREATION ERROR: failed to create shader" << std::endl;
    }

    this->m_textureID = textureManager_.createCubemap(facesTextures);

    if (m_textureID == 0) {
      std::cerr << "ERROR: Failed to create cubemap texture" << std::endl;
      return;
    }

    std::vector skyboxVertices = {
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f
    };
    std::vector<VertexAttribute> attributes = {
      // position
      {0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0},
    };
    bufferHandle_ = bufferManager_.createBufferSet(
      "Skybox",
      skyboxVertices,
      {},
      attributes
    );

    m_initialized = true;
    std::cout << "Skybox initialized successfully!" << std::endl;
    std::cout << "========================\n" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "EXCEPTION in skybox creation: " << e.what() << std::endl;
  }
}

void Skybox::render(const glm::mat4 model, const glm::mat4 view,
                    const glm::mat4 projection) const {
  if (!m_initialized || !m_enabled) {
    return;
  }

  glDepthMask(GL_FALSE);
  glDepthFunc(GL_LEQUAL);  // Change depth function

  m_shader->use();
  glBindVertexArray(bufferHandle_.getVAO());

  // Remove translation from view matrix - this is the key!
  const auto skyboxView =
      glm::mat4(glm::mat3(view));  // Convert to mat3 then back to mat4

  m_shader->setMat4("view", skyboxView);
  m_shader->setMat4("projection", projection);
  m_shader->setInt("skybox", 0);

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDepthFunc(GL_LESS);  // Reset depth function
  glDepthMask(GL_TRUE);

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}