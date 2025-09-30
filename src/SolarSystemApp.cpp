//
// Created by Daniel on 30-Sep-25.
//
#include "SolarSystemApp.h"

#include <GLFW/glfw3.h>

#include <iostream>


// Static initialization
SolarSystemApp* SolarSystemApp::instance_ = nullptr;

const std::vector<std::string> AppConfig::SKYBOX_FACES = {
    "../textures/skybox1.png",
    "../textures/skybox2.png",
    "../textures/skybox3.png",
    "../textures/skybox4.png",
    "../textures/skybox5.png",
    "../textures/skybox6.png"
};

SolarSystemApp::SolarSystemApp() {
    instance_ = this;
}

bool SolarSystemApp::initialize() {
    std::cout << "Initializing Solar System Application..." << std::endl;

    if (!initializeCoreSystems()) {
        std::cerr << "Failed to initialize core systems" << std::endl;
        return false;
    }

    if (!initializePlanets()) {
        std::cerr << "Failed to initialize planets" << std::endl;
        return false;
    }

    if (!initializeRenderers()) {
        std::cerr << "Failed to initialize renderers" << std::endl;
        return false;
    }

    setupInputCallbacks();

    std::cout << "✓ Solar System Application initialized successfully!" << std::endl;
    return true;
}

bool SolarSystemApp::initializeCoreSystems() {
    // Create engine
    engine_ = std::make_unique<Engine>(
        AppConfig::WINDOW_NAME,
        AppConfig::ENABLE_GL_DEPTH_TEST
    );

    // Create and initialize skybox
    skybox_ = std::make_unique<Skybox>(engine_.get(), AppConfig::SKYBOX_FACES);
    if (!skybox_->initialize()) {
        return false;
    }

    // Create Saturn's ring
    saturnRing_ = std::make_unique<Ring>();
    saturnRing_->create(engine_.get(), "../textures/saturn_ring.png");

    // Create text renderer
    textRenderer_ = std::make_unique<TextRenderer>(engine_.get());
    if (!textRenderer_->initialize()) {
        return false;
    }

    // Create planet info panel
    planetInfoPanel_ = std::make_unique<PlanetInfoPanel>(textRenderer_.get());

    return true;
}

bool SolarSystemApp::initializePlanets() {
    // Use factory to create all planets with proper configuration
    celestialBodies_ = CelestialBodyFactory::createSolarSystem(engine_.get());

    if (celestialBodies_.empty()) {
        std::cerr << "Failed to create any planets" << std::endl;
        return false;
    }

    std::cout << "✓ Created " << celestialBodies_.size() << " celestial bodies" << std::endl;
    return true;
}

bool SolarSystemApp::initializeRenderers() {
    sceneRenderer_ = std::make_unique<SceneRenderer>(
        skybox_.get(),
        saturnRing_.get()
    );

    uiRenderer_ = std::make_unique<UIRenderer>(
        textRenderer_.get(),
        planetInfoPanel_.get()
    );

    return true;
}

void SolarSystemApp::setupInputCallbacks() {
    Engine::onLeftClickCallback = &SolarSystemApp::onLeftClickCallbackBridge;
}

void SolarSystemApp::onLeftClickCallbackBridge() {
    if (instance_) {
        instance_->handlePlanetSelection();
    }
}

void SolarSystemApp::handlePlanetSelection() {
    // Gather scales for all planets
    std::vector<glm::vec3> scales;
    scales.reserve(celestialBodies_.size());

    for (const auto& planet : celestialBodies_) {
        scales.push_back(CelestialBodyFactory::getScale(planet.bodyType));
    }

    // Perform ray-sphere intersection test
    auto result = CelestialBodyPicker::pickPlanet(
        engine_->camera,
        celestialBodies_,
        scales
    );

    if (result.hit) {
        selectedPlanetIndex_ = static_cast<int>(result.planetIndex);
        uiRenderer_->setShowPlanetInfo(true);

        std::cout << "✓ Selected planet: "
                  << PlanetInfoPanel::getPlanetInfo(
                         celestialBodies_[result.planetIndex].bodyType).name
                  << std::endl;
    } else {
        selectedPlanetIndex_ = -1;
        uiRenderer_->setShowPlanetInfo(false);
        std::cout << "✓ Deselected planet" << std::endl;
    }
}

void SolarSystemApp::run() {
    std::cout << "Starting main loop..." << std::endl;

    // Pass frame callback to engine
    engine_->render([this]() {
        onFrame();
    });
}

void SolarSystemApp::onFrame() {
    float currentTime = static_cast<float>(glfwGetTime());
    float deltaTime = engine_->deltaTime;

    calculateFPS(currentTime);
    update(deltaTime);
    render(currentTime);
}

void SolarSystemApp::calculateFPS(float currentTime) {
    frameCount_++;

    // Update FPS every 0.5 seconds
    if (currentTime - lastFPSTime_ >= 0.5f) {
        currentFPS_ = frameCount_ / (currentTime - lastFPSTime_);
        frameCount_ = 0;
        lastFPSTime_ = currentTime;
    }
}

void SolarSystemApp::update(float deltaTime) {
    // Update all celestial body orbital positions
    for (auto& planet : celestialBodies_) {
        planet.updateOrbitalPositions(deltaTime * AppConfig::TIME_SCALE);
    }
}

void SolarSystemApp::render(float currentTime) {
    // Create render context
    RenderContext context {
        engine_->camera,
        engine_->SCR_WIDTH,
        engine_->SCR_HEIGHT,
        currentTime
    };

    // Render 3D scene
    sceneRenderer_->renderScene(celestialBodies_, context);

    // Render UI
    uiRenderer_->renderUI(
        currentFPS_,
        engine_->camera,
        engine_->SCR_WIDTH,
        engine_->SCR_HEIGHT
    );

    // Render planet info if selected
    if (selectedPlanetIndex_ >= 0 &&
        selectedPlanetIndex_ < static_cast<int>(celestialBodies_.size())) {

        const auto& selectedPlanet = celestialBodies_[selectedPlanetIndex_];
        PlanetInfo info = PlanetInfoPanel::getPlanetInfo(selectedPlanet.bodyType);

        uiRenderer_->renderPlanetInfo(
            selectedPlanet.position,
            info,
            engine_->camera,
            engine_->SCR_WIDTH,
            engine_->SCR_HEIGHT
        );
    }
}