#include "Engine.h"

#include <utils/debug_utils.h>

#include <AppConfig.h>
#include <celestialbody/CelestialBodyPicker.h>
#include <core/audio/AudioManager.h>
#include <core/input/InputManager.h>
#include <core/window/WindowManager.h>
#include <graphics/buffer/BufferManager.h>
#include <rendering/RenderContext.h>
#include <rendering/renderers/SceneRenderer.h>
#include <rendering/renderers/UIRenderer.h>

bool Engine::canRenderPanel = false;
BodyType Engine::currentSelectedBodyType = Unknown;

Engine::Engine(bool enable_gl_depth_test, BufferManager& bufferManager)
    : context_(std::make_unique<EngineContext>()),
      bufferManager_(bufferManager) {
  try {
    initGLFW();

    context_->inputManager = std::make_unique<InputManager>(
        AppConfig::SCR_WIDTH, AppConfig::SCR_HEIGHT);

    context_->windowManager = std::make_unique<WindowManager>();
    context_->windowManager->create(AppConfig::SCR_WIDTH, AppConfig::SCR_HEIGHT,
                                    AppConfig::WINDOW_NAME, [this] {
                                      context_->inputManager->setInputCallbacks(
                                          context_->windowManager->getWindow());
                                    });

    context_->camera =
        std::make_unique<Camera>(glm::vec3(0.0f, 5.0f, 20.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -15.0f);

    initGLAD();

    context_->textRenderer = std::make_unique<TextRenderer>(
        bufferManager_, AppConfig::SCR_WIDTH, AppConfig::SCR_HEIGHT);
    context_->uiRenderer =
        std::make_unique<UIRenderer>(*context_->textRenderer);
    context_->sceneRenderer = std::make_unique<SceneRenderer>();
    context_->audioManager = std::make_unique<AudioManager>();

    initializeBasicDebugging();

    if (enable_gl_depth_test) {
      GL_CHECK(glEnable(GL_DEPTH_TEST));
      std::cout << "Depth testing enabled" << std::endl;
    }

    // Use culling later
    // GL_CHECK(glEnable(GL_CULL_FACE));
    // GL_CHECK(glCullFace(GL_BACK));
    // GL_CHECK(glFrontFace(GL_CCW));

    setupInputConfig();

    context_->audioManager->playBackgroundMusic("../audio/dnb.mp3");
    context_->audioManager->setVolume(0.5f);
    std::cout << "Engine initialized successfully" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Engine initialization failed: " << e.what() << std::endl;
  }
}

Engine::~Engine() {
  stopEngine = true;

  if (context_->inputManager) {
    std::cout << "\nDestroying Input manager\n" << std::endl;
    context_->inputManager.reset();
  }
  if (context_->audioManager) {
    std::cout << "\nDestroying Audio manager\n" << std::endl;
    context_->audioManager.reset();
  }
  if (context_->textRenderer) {
    std::cout << "\nDestroying text renderer\n" << std::endl;
    context_->textRenderer.reset();
  }
  if (context_->uiRenderer) {
    std::cout << "\nDestroying UI renderer\n" << std::endl;
    context_->uiRenderer.reset();
  }
  if (context_->camera) {
    std::cout << "\nDestroying Camera\n" << std::endl;
    context_->camera.reset();
  }
  if (context_->sceneRenderer) {
    std::cout << "\nDestroying Scene renderer\n" << std::endl;
    context_->sceneRenderer.reset();
  }
  if (context_->windowManager) {
    std::cout << "\nDestroying Window manager\n" << std::endl;
    context_->windowManager.reset();
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

void Engine::run(std::function<void(FrameContext&)> frameCallback,
                 const std::deque<ISceneRenderable*>& renderables) {
  try {
    if (stopEngine) {
      return;
    }

    std::cout << "Running engine loop..." << std::endl;
    FrameContext frameContext;
    context_->windowManager->run(
        [this, &frameCallback, &renderables, &frameContext] {
          if (stopEngine) {
            return;
          }

          frameContext.currentTime = context_->windowManager->getGLFWTime();
          frameContext.deltaTime =
              frameContext.currentTime - frameContext.lastFrame;
          frameContext.lastFrame = frameContext.currentTime;

          frameContext.shouldTerminate = context_->inputManager->processInput(
              context_->windowManager->getWindow(), frameContext.deltaTime);

          render(frameContext.currentTime, renderables);
          calculateFPS(frameContext.currentTime);
          frameCallback(frameContext);
        });
  } catch (const std::exception& e) {
    std::cerr << "Exception appeared when running the engine: " << e.what()
              << std::endl;
  }
}

void Engine::render(float currentTime,
                    const std::deque<ISceneRenderable*>& renderables) const {
  RenderContext renderContext{*context_->camera,    currentSelectedBodyType,
                              AppConfig::SCR_WIDTH, AppConfig::SCR_HEIGHT,
                              currentTime,          currentFPS_,
                              canRenderPanel};
  // Render 3D scene
  context_->sceneRenderer->render(renderables, renderContext);
  // Render UI
  context_->uiRenderer->render(renderContext);
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

void Engine::setupInputConfig() const {
  context_->inputManager->bindKey(GLFW_KEY_W, [&](float dt, float sp) {
    context_->camera->processMovement(FORWARD, dt, sp);
  });
  context_->inputManager->bindKey(GLFW_KEY_S, [&](float dt, float sp) {
    context_->camera->processMovement(BACKWARD, dt, sp);
  });
  context_->inputManager->bindKey(GLFW_KEY_A, [&](float dt, float sp) {
    context_->camera->processMovement(LEFT, dt, sp);
  });
  context_->inputManager->bindKey(GLFW_KEY_D, [&](float dt, float sp) {
    context_->camera->processMovement(RIGHT, dt, sp);
  });
  context_->inputManager->bindKey(GLFW_KEY_Q, [&](float dt, float sp) {
    context_->camera->processMovement(DOWN, dt, sp);
  });
  context_->inputManager->bindKey(GLFW_KEY_E, [&](float dt, float sp) {
    context_->camera->processMovement(UP, dt, sp);
  });

  context_->inputManager->setPointerMovementCallback(
      [this](float xoffset, float yoffset) {
        context_->camera->processPointerMovement(xoffset, yoffset);
      });

  context_->inputManager->setAxisCallback(
      [this](float value) { context_->camera->processAxis(value); });

  context_->inputManager->setPrimaryActionCallback([this]() {
    CelestialBodyPicker::pickBody(
        *context_->camera, [this](const int selectedBodyIndex) {
          if (selectedBodyIndex >= 0) {
            Engine::canRenderPanel = true;
          } else {
            Engine::canRenderPanel = false;
          }
          switch (selectedBodyIndex) {
            case 0: {
              Engine::currentSelectedBodyType = Sun;
              break;
            }
            case 1: {
              Engine::currentSelectedBodyType = Mercury;
              break;
            }
            case 2: {
              Engine::currentSelectedBodyType = Venus;
              break;
            }
            case 3: {
              Engine::currentSelectedBodyType = Earth;
              break;
            }
            case 4: {
              Engine::currentSelectedBodyType = Mars;
              break;
            }
            case 5: {
              Engine::currentSelectedBodyType = Jupiter;
              break;
            }
            case 6: {
              Engine::currentSelectedBodyType = Saturn;
              break;
            }
            case 7: {
              Engine::currentSelectedBodyType = Uranus;
              break;
            }
            case 8: {
              Engine::currentSelectedBodyType = Neptune;
              break;
            }
            default: {
              Engine::currentSelectedBodyType = Unknown;
              break;
            }
          }
        });
  });
  context_->inputManager->setFullscreenActionCallback(
      [this]() { context_->windowManager->toggleFullscreen(); });
}
