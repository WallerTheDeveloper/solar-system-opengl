// Replace your skybox.cpp with this complete working version:

#include "Skybox.h"

#include <iostream>

Skybox::Skybox(Engine* engine, std::vector<std::string> facesTextures)
    : engine(engine),
      m_faces(facesTextures),
      m_boxMeshData(nullptr),
      m_VAO(0),
      m_VBO(0),
      m_EBO(0),
      m_textureID(0),
      m_indexCount(0),
      m_initialized(false),
      m_enabled(true) {}

Skybox::~Skybox() {
  if (m_initialized) {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteTextures(1, &m_textureID);
  }
}

bool Skybox::initialize() {
  if (m_initialized) {
    return true;
  }

  std::cout << "\n=== SKYBOX CREATION ===" << std::endl;

  try {
    m_shader = std::make_unique<Shader>("../shaders/skybox.vert",
                                        "../shaders/skybox.frag");

    if (m_shader == nullptr) {
      std::cerr << "SKYBOX CREATION ERROR: failed to create shader" << std::endl;
    }

    this->m_textureID = engine->createCubemap(this->m_faces);

    if (m_textureID == 0) {
      std::cerr << "ERROR: Failed to create cubemap texture" << std::endl;
      return false;
    }

    float skyboxVertices[] = {
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

    engine->generateVAO(&m_VAO);
    engine->bindVAO(m_VAO);

    engine->generateBuffer(&m_VBO);
    engine->bindBuffer(GL_ARRAY_BUFFER, m_VBO);
    engine->setBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),
                          skyboxVertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    engine->defineVertexLayout(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                               (void*)0);

    engine->unbindVAO();
    engine->unbindVBO();

    m_initialized = true;
    std::cout << "✓ Skybox initialized successfully!" << std::endl;
    std::cout << "========================\n" << std::endl;

    return true;
  } catch (const std::exception& e) {
    std::cerr << "EXCEPTION in skybox creation: " << e.what() << std::endl;
    return false;
  }
}

void Skybox::render(const glm::mat4& view, const glm::mat4& projection) {
  if (!m_initialized || !m_enabled) {
    return;
  }

  glDepthMask(GL_FALSE);
  glDepthFunc(GL_LEQUAL);  // Change depth function

  m_shader->use();
  glBindVertexArray(m_VAO);

  // Remove translation from view matrix - this is the key!
  glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // Convert to mat3 then back to mat4

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