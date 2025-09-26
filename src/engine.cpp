#include "engine.h"

Engine* Engine::instance = nullptr;

Engine::Engine(std::string windowName, bool enable_gl_depth_test)
    : windowName(windowName),
      window(nullptr),
      isInitialized(false),
      camera(glm::vec3(0.0f, 5.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f,
             -15.0f) {
  instance = this;
  try {
    initGLFW();
    window = createWindow(windowName, SCR_WIDTH, SCR_HEIGHT);
    initGLAD();

    initializeBasicDebugging();

    if (enable_gl_depth_test) {
      GL_CHECK(glEnable(GL_DEPTH_TEST));
      std::cout << "Depth testing enabled" << std::endl;
    }

    // Use culling later
    // GL_CHECK(glEnable(GL_CULL_FACE));
    // GL_CHECK(glCullFace(GL_BACK));
    // GL_CHECK(glFrontFace(GL_CCW));

    std::cout << "OpenGL state configured successfully" << std::endl;

    isInitialized = true;
    std::cout << "Engine initialized successfully" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Engine initialization failed: " << e.what() << std::endl;
    isInitialized = false;
  }
}

Engine::~Engine() {
  if (window) {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
  std::cout << "Engine destroyed" << std::endl;
}

void Engine::render(std::function<void(Engine*)> renderCallback,
                    Engine* engine) {
  if (!isInitialized || !window) {
    std::cerr << "Engine not properly initialized. Cannot start render loop."
              << std::endl;
    return;
  }

  std::cout << "Starting render loop..." << std::endl;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    if (renderCallback && engine) {
      renderCallback(engine);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
void Engine::renderIndices(unsigned int VAO, unsigned int indicesCount,
                           bool unbind) {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
  if (unbind) {
    glBindVertexArray(0);
  }
}

Engine& Engine::getInstance() {
  if (!instance) {
    throw std::runtime_error(
        "Engine not initialized! Create Engine instance first.");
  }
  return *instance;
}

bool Engine::isEngineInitialized() { return instance != nullptr; }

void Engine::generateVAO(unsigned int* VAO) { glGenVertexArrays(1, VAO); }

void Engine::generateBuffer(unsigned int* buffer) { glGenBuffers(1, buffer); }

void Engine::bindVAO(unsigned int VAO) { glBindVertexArray(VAO); }

void Engine::bindBuffer(GLenum target, unsigned int buffer) {
  // example: glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(target, buffer);
}

void Engine::setBufferData(GLenum target, GLsizeiptr size, const void* data,
                           GLenum usage) {
  // example VBO: glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
  // GL_STATIC_DRAW); example EBO: glBufferData(GL_ELEMENT_ARRAY_BUFFER,
  // sizeof(indicies), indicies, GL_STATIC_DRAW);
  glBufferData(target, size, data, usage);
}

void Engine::unbindVAO() { glBindVertexArray(0); }
void Engine::unbindVBO() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void Engine::deleteVAO(int size, unsigned int& VAO) {
  glDeleteVertexArrays(size, &VAO);
}

void Engine::deleteBuffers(int size, const unsigned int& buffer) {
  glDeleteBuffers(size, &buffer);
}

void Engine::defineVertexLayout(unsigned int shaderLayoutIndex, int size,
                                GLenum type, GLboolean normalizeData,
                                int stride, const void* offset) {
  // example:   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 *
  // sizeof(float), (void*)(3 * sizeof(float)));
  glVertexAttribPointer(shaderLayoutIndex, size, type, normalizeData, stride,
                        offset);

  glEnableVertexAttribArray(shaderLayoutIndex);
}

SphereMeshData Engine::generateSphereMesh(float radius,
                                          unsigned int sectorCount,
                                          unsigned int stackCount) {
  SphereMeshData data;

  float x, y, z, xy;
  float sectorStep = 2 * M_PI / sectorCount;
  float stackStep = M_PI / stackCount;
  float sectorAngle, stackAngle;

  for (unsigned int i = 0; i <= stackCount; ++i) {
    stackAngle = M_PI / 2 - i * stackStep;
    xy = radius * cosf(stackAngle);
    y = radius * sinf(stackAngle);

    for (unsigned int j = 0; j <= sectorCount; ++j) {
      sectorAngle = j * sectorStep;

      x = xy * cosf(sectorAngle);
      z = xy * sinf(sectorAngle);

      // Position
      data.vertices.push_back(x);
      data.vertices.push_back(y);
      data.vertices.push_back(z);

      // Texture coordinates
      float u = (float)j / sectorCount;
      float v = (float)i / stackCount;
      data.vertices.push_back(u);
      data.vertices.push_back(v);
    }
  }

  unsigned int k1, k2;
  for (unsigned int i = 0; i < stackCount; ++i) {
    k1 = i * (sectorCount + 1);
    k2 = k1 + sectorCount + 1;

    for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
      if (i != 0) {
        data.indices.push_back(k1);
        data.indices.push_back(k2);
        data.indices.push_back(k1 + 1);
      }

      if (i != (stackCount - 1)) {
        data.indices.push_back(k1 + 1);
        data.indices.push_back(k2);
        data.indices.push_back(k2 + 1);
      }
    }
  }

  data.indicesCount = data.indices.size();

  return data;
}

BoxMeshData Engine::generateBoxMesh(float width, float height, float depth) {
  BoxMeshData data;

  float halfWidth = width * 0.5f;
  float halfHeight = height * 0.5f;
  float halfDepth = depth * 0.5f;

  // (x, y, z, u, v)
  std::vector<float> vertices = {
      // Front face
      -halfWidth, -halfHeight, halfDepth, 0.0f, 0.0f,  // Bottom left
      halfWidth, -halfHeight, halfDepth, 1.0f, 0.0f,   // Bottom right
      halfWidth, halfHeight, halfDepth, 1.0f, 1.0f,    // Top right
      -halfWidth, halfHeight, halfDepth, 0.0f, 1.0f,   // Top left

      // Back face
      -halfWidth, -halfHeight, -halfDepth, 1.0f, 0.0f,  // Bottom left
      halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f,   // Bottom right
      halfWidth, halfHeight, -halfDepth, 0.0f, 1.0f,    // Top right
      -halfWidth, halfHeight, -halfDepth, 1.0f, 1.0f,   // Top left

      // Left face
      -halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f,  // Bottom left
      -halfWidth, -halfHeight, halfDepth, 1.0f, 0.0f,   // Bottom right
      -halfWidth, halfHeight, halfDepth, 1.0f, 1.0f,    // Top right
      -halfWidth, halfHeight, -halfDepth, 0.0f, 1.0f,   // Top left

      // Right face
      halfWidth, -halfHeight, -halfDepth, 1.0f, 0.0f,  // Bottom left
      halfWidth, -halfHeight, halfDepth, 0.0f, 0.0f,   // Bottom right
      halfWidth, halfHeight, halfDepth, 0.0f, 1.0f,    // Top right
      halfWidth, halfHeight, -halfDepth, 1.0f, 1.0f,   // Top left

      // Top face
      -halfWidth, halfHeight, -halfDepth, 0.0f, 1.0f,  // Bottom left
      halfWidth, halfHeight, -halfDepth, 1.0f, 1.0f,   // Bottom right
      halfWidth, halfHeight, halfDepth, 1.0f, 0.0f,    // Top right
      -halfWidth, halfHeight, halfDepth, 0.0f, 0.0f,   // Top left

      // Bottom face
      -halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f,  // Bottom left
      halfWidth, -halfHeight, -halfDepth, 1.0f, 0.0f,   // Bottom right
      halfWidth, -halfHeight, halfDepth, 1.0f, 1.0f,    // Top right
      -halfWidth, -halfHeight, halfDepth, 0.0f, 1.0f    // Top left
  };

  std::vector<unsigned int> indices = {// Front face
                                       0, 1, 2, 2, 3, 0,
                                       // Back face
                                       4, 5, 6, 6, 7, 4,
                                       // Left face
                                       8, 9, 10, 10, 11, 8,
                                       // Right face
                                       12, 13, 14, 14, 15, 12,
                                       // Top face
                                       16, 17, 18, 18, 19, 16,
                                       // Bottom face
                                       20, 21, 22, 22, 23, 20};

  data.vertices = vertices;
  data.indices = indices;
  data.indicesCount = indices.size();

  return data;
}

unsigned int Engine::addTextureToObject(string path, GLenum target,
                                        GLint wrapping, GLint filtering) {
  texture = make_unique<Texture>();
  const unsigned int textureID = texture->generateTexture(1, target);

  // examples: GL_REPEAT - wrapping, GL_LINEAR - filtering
  GL_CHECK(texture->setTextureWrappingParamsInt(wrapping));
  GL_CHECK(texture->setTextureFilteringParamsInt(filtering));

  int width = 0, height = 0, numberOfChannels = 0;
  unsigned char* data =
      texture->loadTextureImage(path.c_str(), width, height, numberOfChannels);

  if (data && width > 0 && height > 0) {
    cout << "Texture loaded: " << width << "x" << height << " ("
         << numberOfChannels << " channels)" << endl;

    GLenum format = GL_RGB;
    if (numberOfChannels == 1)
      format = GL_RED;
    else if (numberOfChannels == 3)
      format = GL_RGB;
    else if (numberOfChannels == 4)
      format = GL_RGBA;

    GL_CHECK(texture->specifyTextureImage2D(data, format, width, height, true));
    texture->freeImageData(data);
  } else {
    cout << "Failed to load texture: " << path << " - using fallback color"
         << endl;

    GL_CHECK(texture->specifyTextureImage2D(data, GL_RGB, 1, 1, false));
    if (data) {
      texture->freeImageData(data);
    }
  }

  // checkTextureBinding(GL_TEXTURE0);
  return textureID;
}
void Engine::renderTexture2D(GLenum textureUnit, unsigned int textureID) {
  // example textureUnit - GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2 and so on
  texture->setTextureActive2D(textureUnit, textureID);
}

unsigned int Engine::createCubemap(std::vector<std::string> faces) {
  std::cout << "Creating cubemap with files:" << std::endl;
  for (const auto& face : faces) {
    std::cout << "  " << face << std::endl;
  }

  unsigned int cubemapTexture = this->texture->loadCubemap(faces);
  std::cout << "Cubemap created with ID: " << cubemapTexture << std::endl;

  return cubemapTexture;
}

void Engine::initGLFW() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  std::cout << "GLFW initialized" << std::endl;
}

void Engine::initGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }

  std::cout << "GLAD initialized" << std::endl;
}

void Engine::processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  float speedMultiplier = 1.0f;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    speedMultiplier = 5.0f;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.processKeyboard(FORWARD, deltaTime, speedMultiplier);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.processKeyboard(BACKWARD, deltaTime, speedMultiplier);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.processKeyboard(LEFT, deltaTime, speedMultiplier);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.processKeyboard(RIGHT, deltaTime, speedMultiplier);
  }
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width,
                                       int height) {
  if (!instance) {
    return;
  }
  glViewport(0, 0, width, height);
}

void Engine::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  if (!instance) {
    return;
  }

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (instance->firstMouse) {
    instance->lastMouseX = xpos;
    instance->lastMouseY = ypos;
    instance->firstMouse = false;
  }

  float xoffset = xpos - instance->lastMouseX;
  float yoffset = instance->lastMouseY - ypos;

  instance->lastMouseX = xpos;
  instance->lastMouseY = ypos;

  instance->camera.processMouseMovement(xoffset, yoffset);
}

void Engine::scroll_callback(GLFWwindow* window, double xoffset,
                             double yoffset) {
  if (!instance) {
    return;
  }

  instance->camera.processMouseScroll(static_cast<float>(yoffset));
}

GLFWwindow* Engine::createWindow(std::string name, int width, int height) {
  GLFWwindow* newWindow =
      glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
  if (!newWindow) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(newWindow);
  std::cout << "Window created: " << name << " (" << width << "x" << height
            << ")" << std::endl;

  glfwSetFramebufferSizeCallback(newWindow, framebuffer_size_callback);
  glfwSetCursorPosCallback(newWindow, mouse_callback);
  glfwSetScrollCallback(newWindow, scroll_callback);

  glfwSetInputMode(newWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return newWindow;
}
