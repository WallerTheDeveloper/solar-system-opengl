// Replace your skybox.cpp with this complete working version:

#include "Skybox.h"
#include <iostream>

Skybox::Skybox(Engine* engine, const string& texturePath)
    : engine(engine),
      m_texturePath(texturePath),
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

bool Skybox::create() {
  if (m_initialized) {
    return true;
  }

  std::cout << "\n=== SKYBOX CREATION ===" << std::endl;

  try {
    // Step 1: Create shader
    std::cout << "1. Creating shader..." << std::endl;
    m_shader = std::make_unique<Shader>("../shaders/skybox.vert",
                                        "../shaders/skybox.frag");
    std::cout << "✓ Shader created successfully with ID: " << m_shader->ID << std::endl;

    // Step 2: Create cubemap texture with your renamed files
    std::cout << "2. Creating cubemap texture..." << std::endl;
    this->m_textureID = engine->createCubemap();

    if (m_textureID == 0) {
      std::cerr << "ERROR: Failed to create cubemap texture" << std::endl;
      return false;
    }
    std::cout << "✓ Cubemap texture created with ID: " << m_textureID << std::endl;

    // Step 3: Create skybox geometry
    std::cout << "3. Creating skybox geometry..." << std::endl;
    float skyboxVertices[] = {
        // Positions
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
    engine->setBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    engine->defineVertexLayout(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    engine->unbindVAO();
    engine->unbindVBO();

    std::cout << "✓ VAO ID: " << m_VAO << ", VBO ID: " << m_VBO << std::endl;

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

  // Clear any existing OpenGL errors first
  while (glGetError() != GL_NO_ERROR) { /* clear error queue */ }

  // Change depth function to less equal for skybox (render behind everything)
  glDepthFunc(GL_LEQUAL);

  // Use skybox shader
  m_shader->use();
  m_shader->setInt("skybox", 0);  // Set the uniform

  // Remove translation from view matrix for skybox
  glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

  // Set uniforms
  m_shader->setMat4("view", skyboxView);
  m_shader->setMat4("projection", projection);

  // Bind skybox VAO
  engine->bindVAO(m_VAO);

  // Bind cubemap texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

  // Render skybox
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Restore depth function
  glDepthFunc(GL_LESS);
}