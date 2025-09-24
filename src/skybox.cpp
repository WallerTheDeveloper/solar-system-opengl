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
    std::cout << "Skybox not initialized or enabled" << std::endl;
    return;
  }

  static int renderCount = 0;
  renderCount++;

  // Debug every 60 calls (about once per second)
  bool debugThisFrame = (renderCount % 60 == 0);

  if (debugThisFrame) {
    std::cout << "=== SKYBOX RENDER DEBUG ===" << std::endl;
    std::cout << "Texture ID: " << m_textureID << std::endl;
    std::cout << "VAO ID: " << m_VAO << std::endl;
  }

  // Save current depth settings
  GLboolean depthTestEnabled;
  GLboolean depthWriteEnabled;
  GLint depthFunc;
  glGetBooleanv(GL_DEPTH_TEST, &depthTestEnabled);
  glGetBooleanv(GL_DEPTH_WRITEMASK, &depthWriteEnabled);
  glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);

  // Configure for skybox rendering
  glDepthMask(GL_FALSE);  // Disable depth writing

  // Use skybox shader
  m_shader->use();

  if (debugThisFrame) {
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    std::cout << "Current shader program: " << currentProgram << std::endl;
  }

  // Set uniforms
  glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // Remove translation
  m_shader->setMat4("view", skyboxView);
  m_shader->setMat4("projection", projection);
  m_shader->setInt("skybox", 0);

  // Bind VAO
  glBindVertexArray(m_VAO);

  // Bind cubemap texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

  if (debugThisFrame) {
    GLint boundTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundTexture);
    std::cout << "Bound cubemap texture: " << boundTexture << std::endl;

    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      std::cout << "OpenGL error before draw: " << error << std::endl;
    }
  }

  // Render the skybox
  glDrawArrays(GL_TRIANGLES, 0, 36);

  if (debugThisFrame) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      std::cout << "OpenGL error after draw: " << error << std::endl;
    }
    std::cout << "Skybox rendered successfully" << std::endl;
    std::cout << "===========================" << std::endl;
  }

  // Restore depth settings
  glDepthMask(depthWriteEnabled);

  // Clean up
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}