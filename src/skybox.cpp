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

  try {
    m_shader = std::make_unique<Shader>("../shaders/skybox.vert",
                                        "../shaders/skybox.frag");
    this->m_textureID = engine->createCubemap();
    return true;
    // this->m_boxMeshData =
    //     make_unique<BoxMeshData>(engine->generateBoxMesh(
    //         SKYBOX_WIDTH, SKYBOX_HEIGHT, SKYBOX_DEPTH));

    // engine->generateVAO(&m_VAO);
    // engine->bindVAO(m_VAO);
    //
    // engine->generateBuffer(&m_VBO);
    // engine->generateBuffer(&m_EBO);
    //
    // engine->bindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // engine->setBufferData(GL_ARRAY_BUFFER,
    //                       m_boxMeshData->vertices.size() * sizeof(float),
    //                       m_boxMeshData->vertices.data(), GL_STATIC_DRAW);
    //
    // engine->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    // engine->setBufferData(
    //     GL_ELEMENT_ARRAY_BUFFER,
    //     m_boxMeshData->indices.size() * sizeof(unsigned int),
    //     m_boxMeshData->indices.data(), GL_STATIC_DRAW);
    //
    // engine->defineVertexLayout(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    //                            (void*)0);
    // engine->defineVertexLayout(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    //                            (void*)(3 * sizeof(float)));
    //
    // engine->unbindVAO();
    // engine->unbindVBO();
    //
    // this->m_textureID = engine->addTextureToObject(m_texturePath.c_str(),
    // GL_TEXTURE_2D,
    //                                                GL_CLAMP_TO_EDGE,
    //                                                GL_LINEAR);

    // Set texture uniform
    // m_shader->use();
    // m_shader->setInt("skyboxTexture", 0);
    //
    // m_initialized = true;
    // std::cout << "Skybox initialized successfully with " << m_indexCount
    //           << " indices" << std::endl;
    //
    // return true;

  } catch (const std::exception& e) {
    std::cerr << "Failed to initialize skybox: " << e.what() << std::endl;
    return false;
  }
}

void Skybox::render(const glm::mat4& view, const glm::mat4& projection) {
  if (!m_initialized || !m_enabled) {
    return;
  }

  glDepthMask(GL_FALSE);
  m_shader->use();

  engine->bindVAO(m_VAO);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthMask(GL_TRUE);
  // // Save current depth function
  // GLint depthFunc;
  // glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
  //
  // // Change depth function to less equal for skybox
  // glDepthFunc(GL_LEQUAL);
  //
  // // Disable face culling for skybox (we want to see it from inside)
  // glDisable(GL_CULL_FACE);
  //
  // // Use skybox shader
  // m_shader->use();
  //
  // // Remove translation from view matrix for skybox
  // glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
  //
  // // Set uniforms
  // m_shader->setMat4("view", skyboxView);
  // m_shader->setMat4("projection", projection);
  //
  // // Bind skybox texture
  // engine->renderTexture2D(GL_TEXTURE0, this->m_textureID);
  //
  // // Render skybox
  // engine->renderIndices(m_VAO, m_boxMeshData->indicesCount, true);
  // // Restore face culling
  // glEnable(GL_CULL_FACE);
  //
  // // Restore original depth function
  // glDepthFunc(depthFunc);
}