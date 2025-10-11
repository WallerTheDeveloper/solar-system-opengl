//
// Created by Daniel on 30-Sep-25.
//
#include "SolarSystemApp.h"

#include <AppConfig.h>

#include <core/Engine.h>
#include <core/EngineContext.h>
#include <core/texturing/TextureManager.h> // glad
#include <core/input/InputManager.h> // glfw
#include <core/scene/SceneData.h>
#include <core/window/WindowManager.h>
#include <core/Camera.h>

#include <graphics/buffer/BufferManager.h>
#include <graphics/mesh/MeshGenerator.h>
#include <rendering/renderables/scene/CelestialBody.h>
#include <rendering/renderables/scene/Ring.h>
#include <rendering/renderables/scene/Skybox.h>
#include <rendering/renderables/ui/CelestialBodyInfoPanel.h>
#include <rendering/renderers/SceneRenderer.h>
#include <rendering/renderers/TextRenderer.h>
#include <rendering/renderers/UIRenderer.h>

#include <celestialbody/CelestialBodyFactory.h>

#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <iostream>


class SceneData;

const std::vector<std::string> AppConfig::SKYBOX_FACES = {
    "../textures/skybox1.png",
    "../textures/skybox2.png",
    "../textures/skybox3.png",
    "../textures/skybox4.png",
    "../textures/skybox5.png",
    "../textures/skybox6.png"
};

SolarSystemApp::SolarSystemApp() = default;

SolarSystemApp::~SolarSystemApp() {
  if (!isCleanedUp) {
    std::cout << "Warning: cleanup() was not called explicitly, cleaning up in "
                 "destructor\n";
    shutdown();
  }
}

bool SolarSystemApp::initialize() {
    std::cout << "Initializing Solar System Application..." << std::endl;


    bufferManager_ = std::make_unique<BufferManager>();
    textureManager_ = std::make_unique<TextureManager>();
    meshGenerator_ = std::make_unique<MeshGenerator>();

    if (!initializeCoreSystems(*bufferManager_, *textureManager_, celestialBodies_, *textRenderer_, *bodyInfoPanel_)) {
      std::cerr << "Failed to initialize core systems" << std::endl;
      return false;
    }


    if (!initializePlanets(*bufferManager_, *textureManager_, *meshGenerator_)) {
      std::cerr << "Failed to initialize planets" << std::endl;
      return false;
    }

    // if (!initializeRenderers(celestialBodies_, *textRenderer_, *bodyInfoPanel_)) {
    //     std::cerr << "Failed to initialize renderers" << std::endl;
    //     return false;
    // }

    skybox_->create(AppConfig::SKYBOX_FACES);
    saturnRing_->create("../textures/saturn_ring.png");

    setupInputConfig();

    std::cout << "Solar System Application initialized successfully!" << std::endl;
    return true;
}

bool SolarSystemApp::initializeCoreSystems(
    BufferManager& bufferManager, TextureManager& textureManager,
    const std::vector<std::unique_ptr<CelestialBody>>& celestialBodies,
    TextRenderer& textRenderer, CelestialBodyInfoPanel& celestialBodyInfo) {
  windowManager_ = std::make_unique<WindowManager>();
  camera_ =
      std::make_unique<Camera>(glm::vec3(0.0f, 5.0f, 20.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -15.0f);
  inputManager_ = std::make_unique<InputManager>(AppConfig::SCR_WIDTH,
      AppConfig::SCR_HEIGHT);

  sceneRenderer_ = std::make_unique<SceneRenderer>();
  uiRenderer_ = std::make_unique<UIRenderer>(celestialBodies, textRenderer, celestialBodyInfo);
  sceneData_ = std::make_unique<SceneData>();

  EngineContext engineContext{
    (std::move(windowManager_)), (std::move(camera_)),
    (std::move(inputManager_)),  (std::move(sceneRenderer_)),
    (std::move(uiRenderer_)),    (std::move(sceneData_))};

  engine_ =
      std::make_unique<Engine>(engineContext, AppConfig::ENABLE_GL_DEPTH_TEST);

  skybox_ = std::make_unique<Skybox>(bufferManager, textureManager);

  saturnRing_ = std::make_unique<Ring>(bufferManager, textureManager);

  textRenderer_ = std::make_unique<TextRenderer>(bufferManager, AppConfig::SCR_WIDTH, AppConfig::SCR_HEIGHT);

  bodyInfoPanel_ =
      std::make_unique<CelestialBodyInfoPanel>(*textRenderer_);

  std::cout << "CORE SYSTEMS INITIALIZED SUCCESSFULLY" << std::endl;
  return true;
}

bool SolarSystemApp::initializePlanets(BufferManager& bufferManager,
                                       TextureManager& textureManager,
                                       MeshGenerator& meshGenerator) {
  celestialBodies_ = CelestialBodyFactory::createSolarSystem(
      bufferManager, meshGenerator, textureManager);

    if (celestialBodies_.empty()) {
        std::cerr << "Failed to create any planets" << std::endl;
        return false;
    }

    std::cout << "Created " << celestialBodies_.size() << " celestial bodies" << std::endl;
    return true;
}

// bool SolarSystemApp::initializeRenderers(
//     const std::vector<std::unique_ptr<CelestialBody>>& celestialBodies,
//     TextRenderer& textRenderer, CelestialBodyInfoPanel& celestialBodyInfo)
// {
//     sceneRenderer_ = std::make_unique<SceneRenderer>();
//     uiRenderer_ = std::make_unique<UIRenderer>(celestialBodies, textRenderer, celestialBodyInfo);
//
//     return true;
// }
void SolarSystemApp::setupInputConfig() {
  inputManager_.get()->bindKey(GLFW_KEY_W, [&](float dt, float sp) {
    camera_->processMovement(FORWARD, dt, sp);
  });
  inputManager_.get()->bindKey(GLFW_KEY_S, [&](float dt, float sp) {
    camera_->processMovement(BACKWARD, dt, sp);
  });
  inputManager_.get()->bindKey(GLFW_KEY_A, [&](float dt, float sp) {
    camera_->processMovement(LEFT, dt, sp);
  });
  inputManager_.get()->bindKey(GLFW_KEY_D, [&](float dt, float sp) {
    camera_->processMovement(RIGHT, dt, sp);
  });
  inputManager_.get()->bindKey(GLFW_KEY_Q, [&](float dt, float sp) {
    camera_->processMovement(DOWN, dt, sp);
  });
  inputManager_.get()->bindKey(GLFW_KEY_E, [&](float dt, float sp) {
    camera_->processMovement(UP, dt, sp);
  });

  Camera* cam = camera_.get();
  inputManager_.get()->setPointerMovementCallback(
      [cam](float xoffset, float yoffset) {
        cam->processPointerMovement(xoffset, yoffset);
      });

  inputManager_.get()->setAxisCallback(
      [cam](float value) { cam->processAxis(value); });

  inputManager_.get()->setPrimaryActionCallback([this, cam]() {
    handlePlanetSelection(*cam);
  });
}

void SolarSystemApp::handlePlanetSelection(const Camera& camera) {
    // Gather scales for all planets
    std::vector<glm::vec3> scales;
    scales.reserve(celestialBodies_.size());

    for (const auto& body : celestialBodies_) {
      scales.push_back(CelestialBodyFactory::getScale(body->getBodyProps().type));
    }

    // Perform ray-sphere intersection test
    // auto result = CelestialBodyPicker::pickPlanet(
    // camera,
    //     celestialBodies_,
    //     scales
    // );
    //
    // if (result.hit) {
    //     selectedPlanetIndex_ = static_cast<int>(result.planetIndex);
    //     uiRenderer_->setShowCelestialBodyInfo(true);
    //
    //     std::cout << "✓ Selected planet: "
    //               << CelestialBodyInfoPanel::getBodyInfo(
    //                      celestialBodies_[result.planetIndex]->type).name
    //               << std::endl;
    // } else {
    //     selectedPlanetIndex_ = -1;
    //     uiRenderer_->setShowCelestialBodyInfo(false);
    //     std::cout << "✓ Deselected planet" << std::endl;
    // }
}

void SolarSystemApp::run() {
  engine_->run([this](const Engine::FrameContext& frameContext) {
    for (auto& body : celestialBodies_) {
        body->updateOrbitalPositions(frameContext.deltaTime * AppConfig::TIME_SCALE);
    }

    if (frameContext.shouldTerminate && !isCleanedUp) {
      shutdown();  // Clean up BEFORE setting window should close
      glfwSetWindowShouldClose(windowManager_.get()->getWindow(), true);
    }
  });
}

void SolarSystemApp::shutdown() {
  if (isCleanedUp) {
    std::cout << "Already cleaned up, skipping..." << std::endl;
    return;
  }

  std::cout << "\n=== Starting Solar System Cleanup ===\n";

  if (skybox_) {
    std::cout << "\nDestroying skybox...\n" << std::endl;
    skybox_.reset();
  }

  if (textRenderer_) {
    std::cout << "\nDestroying text renderer...\n" << std::endl;
    textRenderer_.reset();
  }

  if (!celestialBodies_.empty()) {
    std::cout << "\nDestroying " << celestialBodies_.size()
              << " celestial bodies...\n" << std::endl;
    celestialBodies_.clear();
  }

  if (saturnRing_) {
    std::cout << "\nDestroying saturn ring...\n" << std::endl;
    saturnRing_.reset();
  }

  if (meshGenerator_) {
    std::cout << "\nDestroying mesh generator...\n" << std::endl;
    meshGenerator_.reset();
  }

  if (bufferManager_) {
    std::cout << "\nDestroying buffer manager...\n" << std::endl;
    bufferManager_.reset();
  }

  if (windowManager_) {
    std::cout << "\nDestroying window manager...\n" << std::endl;
    windowManager_.reset();
  }

  std::cout << "=== Solar System Cleanup Complete ===\n\n";

  isCleanedUp = true;
}