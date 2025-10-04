#include "Engine.h"

Engine* Engine::instance = nullptr;
std::function<void()> Engine::onLeftClickCallback = nullptr;

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

void Engine::render(const std::function<void()>& renderCallback) {
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

    if (renderCallback) {
      renderCallback();
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
GLFWwindow* Engine::getWindow() {
  return window;
}

Engine& Engine::getInstance() {
  if (!instance) {
    throw std::runtime_error(
        "Engine not initialized! Create Engine instance first.");
  }
  return *instance;
}

bool Engine::isEngineInitialized() { return instance != nullptr; }

unsigned int Engine::addTextureToObject(std::string path, GLenum target,
                                        GLint wrapping, GLint filtering) {
  texture = std::make_unique<Texture>();
  const unsigned int textureID = texture->generateTexture(1, target);

  // examples: GL_REPEAT - wrapping, GL_LINEAR - filtering
  GL_CHECK(texture->setTextureWrappingParamsInt(wrapping));
  GL_CHECK(texture->setTextureFilteringParamsInt(filtering));

  int width = 0, height = 0, numberOfChannels = 0;
  unsigned char* data =
      texture->loadTextureImage(path.c_str(), width, height, numberOfChannels);

  if (data && width > 0 && height > 0) {
    std::cout << "Texture loaded: " << width << "x" << height << " ("
         << numberOfChannels << " channels)" << std::endl;

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
    std::cout << "Failed to load texture: " << path << " - using fallback color"
         << std::endl;

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
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    camera.processKeyboard(UP, deltaTime, speedMultiplier);
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    camera.processKeyboard(DOWN, deltaTime, speedMultiplier);
  }
}
void Engine::toggleFullscreen() {
  if (!window) {
    return;
  }

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  if (!isFullscreen) {
    glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
    glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

    std::cout << "Switched to fullscreen mode (" << mode->width << "x" << mode->height << ")" << std::endl;
    isFullscreen = true;
  } else {
    glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);

    std::cout << "Switched to windowed mode (" << windowedWidth << "x" << windowedHeight << ")" << std::endl;
    isFullscreen = false;
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
void Engine::mouse_button_callback(GLFWwindow* window, int button, int action,
                                   int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (onLeftClickCallback) {
      onLeftClickCallback();
    }
  }
}
void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action,
                          int mods) {
  if (!instance) {
    return;
  }

  // Check for SHIFT + ENTER to toggle fullscreen
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && (mods & GLFW_MOD_SHIFT)) {
    instance->toggleFullscreen();
  }
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
  glfwSetMouseButtonCallback(newWindow, mouse_button_callback);
  glfwSetKeyCallback(newWindow, key_callback);

  glfwSetInputMode(newWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return newWindow;
}
