//#include "Skybox.h"
//
//#include <cmath>
//#include <iostream>
//
//#ifndef M_PI
//#define M_PI 3.14159265358979323846
//#endif
//
//Skybox::Skybox(const std::string& textureFilename)
//    : m_textureFilename(textureFilename),
//      m_VAO(0),
//      m_VBO(0),
//      m_EBO(0),
//      m_textureID(0),
//      m_indexCount(0),
//      m_initialized(false),
//      m_enabled(true) {}
//
//Skybox::~Skybox() {
//  if (m_initialized) {
//    glDeleteVertexArrays(1, &m_VAO);
//    glDeleteBuffers(1, &m_VBO);
//    glDeleteBuffers(1, &m_EBO);
//    glDeleteTextures(1, &m_textureID);
//  }
//}
//
//bool Skybox::initialize(Engine* engine) {
//    if (m_initialized) 
//    {
//        return true;
//    }
//
//    try 
//    {
//        m_shader = std::make_unique<Shader>("shaders/skybox.vert", "shaders/skybox.frag");
//        m_textureLoader = std::make_unique<Texture>();
//
//        SphereData sphereData = engine->generateSphere(SKYBOX_RADIUS, SKYBOX_SECTORS, SKYBOX_STACKS);
//        m_vertices.clear();
//        m_indices.clear();
//        m_indexCount = sphereData.indices.size();
//
//        m_indices = sphereData.indices;
//        m_vertices = sphereData.vertices;
//
//        // Setup OpenGL buffers
//        setupBuffers();
//
//        engine->generateVAO();
//        BufferConfig config;
//        config.useVAO = true;
//        config.useVBO = true;
//        config.useEBO = true;
//        config.vertexDataSize = m_vertices.size() * sizeof();
//        config.vertexData = ;
//        engine->setupBuffers(config);
//        engine->setupVertexAttribPointer();
//        // Load texture
//        if (!loadTexture()) {
//            std::cerr << "Failed to load skybox texture: " << m_textureFilename
//                    << std::endl;
//            return false;
//        }
//
//        // Set texture uniform
//        m_shader->use();
//        m_shader->setInt("skyboxTexture", 0);
//
//        m_initialized = true;
//        std::cout << "Skybox initialized successfully with " << m_indexCount
//                    << " indices" << std::endl;
//
//        return true;
//    } 
//    catch (const std::exception& e) 
//    {
//        std::cerr << "Failed to initialize skybox: " << e.what() << std::endl;
//        return false;
//    }
//}
//
//void Skybox::setupBuffers() {
//  // Generate buffers
//  glGenVertexArrays(1, &m_VAO);
//  glGenBuffers(1, &m_VBO);
//  glGenBuffers(1, &m_EBO);
//
//  // Bind VAO
//  glBindVertexArray(m_VAO);
//
//  // Bind and fill VBO
//  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
//               m_vertices.data(), GL_STATIC_DRAW);
//
//  // Bind and fill EBO
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
//  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
//               m_indices.data(), GL_STATIC_DRAW);
//
//  // Position attribute (location 0)
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//  glEnableVertexAttribArray(0);
//
//  // Texture coordinate attribute (location 1)
//  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
//                        (void*)(3 * sizeof(float)));
//  glEnableVertexAttribArray(1);
//
//  // Unbind
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
//  glBindVertexArray(0);
//}
//
//bool Skybox::loadTexture() {
//    m_textureLoader->generateTexture(1, m_textureID);
//
//    int width, height, numberOfChannels;
//    unsigned char* data = m_textureLoader->loadTextureImage(m_textureFilename.c_str(), width, height, numberOfChannels);
//
//    if (!data) 
//    {
//        std::cerr << "Failed to load skybox texture image: " << m_textureFilename
//                    << std::endl;
//        return false;
//    }
//
//    m_textureLoader->setTextureWrappingParamsInt(GL_CLAMP_TO_EDGE);
//    m_textureLoader->setTextureFilteringParamsInt(GL_LINEAR);
//
//    GLenum format = (numberOfChannels == 4) ? GL_RGBA : GL_RGB;
//
//    m_textureLoader->specifyTextureImage2D(data, format, width, height, true);
//
//    m_textureLoader->freeImageData(data);
//
//    std::cout << "Skybox texture loaded: " << width << "x" << height << " ("
//            << numberOfChannels << " channels)" << std::endl;
//
//    return true;
//}
//
//void Skybox::render(const glm::mat4& view, const glm::mat4& projection) {
//  if (!m_initialized || !m_enabled) {
//    return;
//  }
//
//  // Save current depth function
//  GLint depthFunc;
//  glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
//
//  // Change depth function to less equal for skybox
//  glDepthFunc(GL_LEQUAL);
//
//  // Disable face culling for skybox (we want to see it from inside)
//  glDisable(GL_CULL_FACE);
//
//  // Use skybox shader
//  m_shader->use();
//
//  // Remove translation from view matrix for skybox
//  glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
//
//  // Set uniforms
//  m_shader->setMat4("view", skyboxView);
//  m_shader->setMat4("projection", projection);
//
//  // Bind skybox texture
//  glActiveTexture(GL_TEXTURE0);
//  glBindTexture(GL_TEXTURE_2D, m_textureID);
//
//  // Render skybox
//  glBindVertexArray(m_VAO);
//  glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
//  glBindVertexArray(0);
//
//  // Restore face culling
//  glEnable(GL_CULL_FACE);
//
//  // Restore original depth function
//  glDepthFunc(depthFunc);
//}