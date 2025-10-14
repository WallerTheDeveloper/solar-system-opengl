//
// Created by Daniel on 30-Sep-25.
//
#include "SolarSystemApp.h"

#include <AppConfig.h>

#include <core/Engine.h>
#include <core/texturing/TextureManager.h> // glad
#include <core/input/InputManager.h> // glfw

#include <graphics/buffer/BufferManager.h>
#include <graphics/mesh/MeshGenerator.h>
#include <rendering/renderables/scene/CelestialBody.h>
#include <rendering/renderables/scene/Skybox.h>

#include <celestialbody/CelestialBodyFactory.h>

#include <iostream>

const std::vector<std::string> AppConfig::SKYBOX_FACES = {
    "../textures/skybox1.png",
    "../textures/skybox2.png",
    "../textures/skybox3.png",
    "../textures/skybox4.png",
    "../textures/skybox5.png",
    "../textures/skybox6.png"
};

SolarSystemApp::SolarSystemApp() {

}

SolarSystemApp::~SolarSystemApp() {
  shutdown();
}

bool SolarSystemApp::initialize() {
  try {
    std::cout << "Initializing Solar System Application..." << std::endl;

    bufferManager_ = std::make_unique<BufferManager>();
    textureManager_ = std::make_unique<TextureManager>();
    meshGenerator_ = std::make_unique<MeshGenerator>();

    engine_ = std::make_unique<Engine>(AppConfig::ENABLE_GL_DEPTH_TEST,
                                       *bufferManager_);

    skybox_ = std::make_unique<Skybox>(*bufferManager_, *textureManager_);
    renderables_.push_back(std::move(skybox_));

    if (!initializePlanets(*bufferManager_, *textureManager_,
                           *meshGenerator_)) {
      std::cerr << "Failed to initialize planets" << std::endl;
      return false;
    }

    std::cout << "Solar System Application initialized successfully!"
              << std::endl;
    return true;
  } catch (std::exception& e) {
    std::cerr << "Failed to initialize Solar System Application" << e.what()
              << std::endl;
    return false;
  }
}

bool SolarSystemApp::initializePlanets(BufferManager& bufferManager,
                                       TextureManager& textureManager,
                                       MeshGenerator& meshGenerator) {
  std::vector<std::unique_ptr<CelestialBody>> celestialBodies = CelestialBodyFactory::createSolarSystem(
      bufferManager, meshGenerator, textureManager);

    if (celestialBodies.empty()) {
        std::cerr << "Failed to create any planets" << std::endl;
        return false;
    }

    std::cout << "Created " << celestialBodies.size() << " celestial bodies" << std::endl;

    for (auto& body : celestialBodies) {
      renderables_.push_back(std::move(body));
    }

    return true;
}

void SolarSystemApp::handlePlanetSelection(const Camera& camera) {
    // Gather scales for all planets
    // std::vector<glm::vec3> scales;
    // scales.reserve(celestialBodies_.size());
    //
    // for (const auto& body : celestialBodies_) {
    //   scales.push_back(CelestialBodyFactory::getScale(body->getBodyProps().type));
    // }

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
    for (const auto& body : renderables_) {
        body->update(frameContext.deltaTime * AppConfig::TIME_SCALE);
    }

    if (frameContext.shouldTerminate) {
      shutdown();
      // glfwSetWindowShouldClose(, true);
    }
  }, renderables_);
}

void SolarSystemApp::shutdown() {
  std::cout << "\n=== Starting Solar System Cleanup ===\n";

  if (skybox_) {
    std::cout << "\nDestroying skybox...\n" << std::endl;
    skybox_.reset();
  }

  if (engine_) {
    std::cout << "\nDestroying engine...\n" << std::endl;
    engine_.reset();
  }

  if (!renderables_.empty()) {
    std::cout << "\nDestroying " << renderables_.size()
              << " renderables...\n" << std::endl;
    renderables_.clear();
  }

  // if (saturnRing_) {
  //   std::cout << "\nDestroying saturn ring...\n" << std::endl;
  //   saturnRing_.reset();
  // }

  if (meshGenerator_) {
    std::cout << "\nDestroying mesh generator...\n" << std::endl;
    meshGenerator_.reset();
  }

  if (bufferManager_) {
    std::cout << "\nDestroying buffer manager...\n" << std::endl;
    bufferManager_.reset();
  }

  std::cout << "=== Solar System Cleanup Complete ===\n\n";
}