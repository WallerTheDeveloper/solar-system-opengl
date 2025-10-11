#include "Engine.h"

#include <utils/debug_utils.h>

#include <core/input/InputManager.h>
#include <core/window/WindowManager.h>

#include <rendering/RenderContext.h>
#include <rendering/renderers/SceneRenderer.h>
#include <rendering/renderers/UIRenderer.h>

Engine::Engine(const EngineContext& engineContext, bool enable_gl_depth_test)
    : engineContext_(engineContext),
      isInitialized(false) {
  try {
    initGLFW();

    engineContext_.WindowManager->create(
        AppConfig::SCR_WIDTH, AppConfig::SCR_HEIGHT, AppConfig::WINDOW_NAME,
        [this, &engineContext] {
          engineContext.InputManager.get()->setInputCallbacks(engineContext.WindowManager.get()->getWindow());
        });

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

void Engine::calculateFPS(float currentTime) {
  frameCount_++;

  // Update FPS every 0.5 seconds
  if (currentTime - lastFPSTime_ >= 0.5f) {
    currentFPS_ = frameCount_ / (currentTime - lastFPSTime_);
    frameCount_ = 0;
    lastFPSTime_ = currentTime;
  }
}

void Engine::run(std::function<void(FrameContext&)> frameCallback) {
  if (!isInitialized) {
    std::cerr << "Engine not properly initialized. Cannot start render loop."
              << std::endl;
    return;
  }

  std::cout << "Running engine loop..." << std::endl;

  engineContext_.WindowManager.get()->run([this, &frameCallback] {
    FrameContext frameContext;

    frameContext.currentTime =
        engineContext_.WindowManager.get()->getGLFWTime();
    frameContext.deltaTime = frameContext.currentTime - frameContext.lastFrame;
    frameContext.lastFrame = frameContext.currentTime;

    frameContext.shouldTerminate =
        engineContext_.InputManager.get()->processInput(
            engineContext_.WindowManager.get()->getWindow(),
            frameContext.deltaTime);

    render(frameContext.currentTime);
    calculateFPS(frameContext.currentTime);
    frameCallback(frameContext);
  });
}

void Engine::render(float currentTime) const {
  RenderContext context{*engineContext_.Camera.get(), AppConfig::SCR_WIDTH, AppConfig::SCR_HEIGHT,
                        currentTime, currentFPS_};
  // Render 3D scene
  engineContext_.SceneRenderer->render(engineContext_.Renderables, context);

  // Render UI
  engineContext_.UiRenderer->render(context);
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