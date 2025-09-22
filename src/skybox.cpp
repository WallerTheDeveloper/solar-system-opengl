#include "Skybox.h"

#include <iostream>

Skybox::Skybox(Engine* engine, const string& texturePath)
    : engine(engine),
      m_texturePath(texturePath),
      m_sphereMeshData(nullptr),
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

  try {
    m_shader = std::make_unique<Shader>("../shaders/skybox.vert",
                                        "../shaders/skybox.frag");

    this->m_sphereMeshData =
        make_unique<SphereMeshData>(engine->generateSphereMesh(
            SKYBOX_RADIUS, SKYBOX_SECTORS, SKYBOX_STACKS));

    engine->generateVAO(&m_VAO);
    engine->bindVAO(m_VAO);

    engine->generateBuffer(&m_VBO);
    engine->generateBuffer(&m_EBO);

    engine->bindBuffer(GL_ARRAY_BUFFER, m_VBO);
    engine->setBufferData(GL_ARRAY_BUFFER,
                          m_sphereMeshData->vertices.size() * sizeof(float),
                          m_sphereMeshData->vertices.data(), GL_STATIC_DRAW);

    engine->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    engine->setBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        m_sphereMeshData->indices.size() * sizeof(unsigned int),
        m_sphereMeshData->indices.data(), GL_STATIC_DRAW);

    engine->defineVertexLayout(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                               (void*)0);
    engine->defineVertexLayout(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                               (void*)(3 * sizeof(float)));

    engine->unbindVAO();
    engine->unbindVBO();

    this->m_textureID = engine->addTextureToObject(m_texturePath.c_str(),
                                                   GL_CLAMP_TO_EDGE, GL_LINEAR);

    // Set texture uniform
    m_shader->use();
    m_shader->setInt("skyboxTexture", 0);

    m_initialized = true;
    std::cout << "Skybox initialized successfully with " << m_indexCount
              << " indices" << std::endl;

    return true;
  } catch (const std::exception& e) {
    std::cerr << "Failed to initialize skybox: " << e.what() << std::endl;
    return false;
  }
}

// bool Skybox::loadTexture() {
//   m_textureLoader->generateTexture(1, m_textureID);
//
//   int width, height, numberOfChannels;
//   unsigned char* data = m_textureLoader->loadTextureImage(
//       m_textureFilename.c_str(), width, height, numberOfChannels);
//
//   if (!data) {
//     std::cerr << "Failed to load skybox texture image: " << m_textureFilename
//               << std::endl;
//     return false;
//   }
//
//   m_textureLoader->setTextureWrappingParamsInt(GL_CLAMP_TO_EDGE);
//   m_textureLoader->setTextureFilteringParamsInt(GL_LINEAR);
//
//   GLenum format = (numberOfChannels == 4) ? GL_RGBA : GL_RGB;
//
//   m_textureLoader->specifyTextureImage2D(data, format, width, height, true);
//
//   m_textureLoader->freeImageData(data);
//
//   std::cout << "Skybox texture loaded: " << width << "x" << height << " ("
//             << numberOfChannels << " channels)" << std::endl;
//
//   return true;
// }

void Skybox::render(const glm::mat4& view, const glm::mat4& projection) {
  if (!m_initialized || !m_enabled) {
    return;
  }

  // Save current depth function
  GLint depthFunc;
  glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);

  // Change depth function to less equal for skybox
  glDepthFunc(GL_LEQUAL);

  // Disable face culling for skybox (we want to see it from inside)
  glDisable(GL_CULL_FACE);

  // Use skybox shader
  m_shader->use();

  // Remove translation from view matrix for skybox
  glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

  // Set uniforms
  m_shader->setMat4("view", skyboxView);
  m_shader->setMat4("projection", projection);

  // Bind skybox texture
  engine->renderTexture2D(GL_TEXTURE0, this->m_textureID);

  // Render skybox
  engine->renderIndices(m_VAO, m_sphereMeshData->indicesCount, true);
  // Restore face culling
  glEnable(GL_CULL_FACE);

  // Restore original depth function
  glDepthFunc(depthFunc);
}