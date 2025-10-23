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

SolarSystemApp::SolarSystemApp() = default;

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
    renderables_.push_back(skybox_.get());

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
  CelestialBodyFactory::createSolarSystem(bufferManager, meshGenerator,
                                          textureManager);

  auto& bodies = CelestialBodyFactory::getCelestialBodies();
  if (bodies.empty()) {
    std::cerr << "Failed to create any planets" << std::endl;
    return false;
  }

  for (const auto& body : bodies) {
    renderables_.push_back(body.get());
  }

  std::cout << "Created " << bodies.size() << " celestial bodies"
            << std::endl;

  return true;
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

  if (!renderables_.empty()) {
    std::cout << "\nDestroying " << renderables_.size()
              << " renderables...\n" << std::endl;
    renderables_.clear();
  }
  CelestialBodyFactory::clear();

  if (meshGenerator_) {
    std::cout << "\nDestroying mesh generator...\n" << std::endl;
    meshGenerator_.reset();
  }

  if (engine_) {
    std::cout << "\nDestroying engine...\n" << std::endl;
    engine_.reset();
  }

  if (bufferManager_) {
    std::cout << "\nDestroying buffer manager...\n" << std::endl;
    bufferManager_.reset();
  }

  std::cout << "=== Solar System Cleanup Complete ===\n\n";
}