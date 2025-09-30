// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <iostream>
#include "engine.h"
#include "planet.h"
#include "shader.h"
#include "skybox.h"
#include "celestialbody.h"
#include "ring.h"
#include "textrenderer.h"
#include "planetinfopanel.h"
#include "planetpicker.h"

#include <memory>

// ------------- SIMULATION SETTINGS -------------
const std::string WINDOW_NAME = "Solar System Simulation";
const bool ENABLE_GL_DEPTH_TEST = true;
const float TIME_SCALE = 1.5f;

static float lastFPSTime = 0.0f;
static int frameCount = 0;
static float currentFPS = 0.0f;

std::unique_ptr<Engine> engine;
std::unique_ptr<Skybox> skybox;
std::unique_ptr<Ring> saturnRing;
std::unique_ptr<TextRenderer> textRenderer;

std::vector<Planet> celestialBodies;

std::vector<std::string> faces
{
  "../textures/skybox1.png",
  "../textures/skybox2.png",
  "../textures/skybox3.png",
  "../textures/skybox4.png",
  "../textures/skybox5.png",
  "../textures/skybox6.png"
};

void onRender();
void renderUI();
void renderPlanetInfo(int planetIndex);

float getPlanetsRotationSpeed(CelestialBody::BodyType body);
glm::vec3 getPlanetScale(CelestialBody::BodyType body);

// Global state for selection
std::unique_ptr<PlanetInfoPanel> planetInfoPanel;
int selectedPlanetIndex = -1;

void createCoreSystems() {
  engine = std::make_unique<Engine>(WINDOW_NAME, ENABLE_GL_DEPTH_TEST);
  skybox = std::make_unique<Skybox>(engine.get(), faces);
  saturnRing = std::make_unique<Ring>();
  textRenderer = std::make_unique<TextRenderer>(engine.get());
  planetInfoPanel = std::make_unique<PlanetInfoPanel>(textRenderer.get());
}

bool initializeCoreSystems() {
  if (!skybox->initialize()) {
    std::cerr << "Failed to create skybox!" << std::endl;
    return true;
  }

  saturnRing->create(engine.get(), "../textures/saturn_ring.png");
  if (!textRenderer->initialize()) {
    std::cerr << "Failed to initialize text renderer!" << std::endl;
    return true;
  }
  return false;
}
int main() {
  try {
    createCoreSystems();
    initializeCoreSystems();

    celestialBodies.emplace_back(
        engine.get(), CelestialBody::Sun, 1.989e30f, 696340000.0f, 0.0f, 0.0f, 0.0f,
        0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[0].create("../textures/sun.jpg");

    celestialBodies.emplace_back(
        engine.get(), CelestialBody::Mercury, 3.301e23f, 2439700.0f, 4.0f, 0.205f,
        20.0f, 0.0f, glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[1].create("../textures/mercury.jpg");

    celestialBodies.emplace_back(
        engine.get(), CelestialBody::Venus, 4.867e24f, 6051800.0f, 6.0f, 0.007f,
        35.0f, 0.0f, glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[2].create("../textures/venus.jpg");

    celestialBodies.emplace_back(
        engine.get(), CelestialBody::Earth, 5.972e24f, 6371000.0f, 8.0f, 0.017f,
        50.0f, 0.0f, glm::vec3(8.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[3].create("../textures/earth.jpg");

    celestialBodies.emplace_back(
        engine.get(), CelestialBody::Mars, 6.417e23f, 3389500.0f, 10.5f, 0.094f,
        75.0f, 0.0f, glm::vec3(10.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[4].create("../textures/mars.jpg");

    celestialBodies.emplace_back(engine.get(), CelestialBody::Jupiter, 1.898e27f,
                                 69911000.0f, 16.0f, 0.049f, 120.0f, 0.0f,
                                 glm::vec3(16.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[5].create("../textures/jupiter.jpg");

    celestialBodies.emplace_back(engine.get(), CelestialBody::Saturn, 5.683e26f,
                                 58232000.0f, 20.0f, 0.057f, 180.0f, 0.0f,
                                 glm::vec3(20.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[6].create("../textures/saturn.jpg");

    celestialBodies.emplace_back(engine.get(), CelestialBody::Uranus, 8.681e25f,
                                 25362000.0f, 25.0f, 0.046f, 250.0f, 0.0f,
                                 glm::vec3(25.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[7].create("../textures/uranus.jpg");

    celestialBodies.emplace_back(engine.get(), CelestialBody::Neptune, 1.024e26f,
                                 24622000.0f, 30.0f, 0.009f, 350.0f, 0.0f,
                                 glm::vec3(30.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[8].create("../textures/neptune.jpg");

    // Setup mouse click callback
    Engine::onLeftClickCallback = []() {
      std::vector<glm::vec3> scales;
      for (const auto& body : celestialBodies) {
        scales.push_back(getPlanetScale(body.bodyType));
      }

      auto result =
          PlanetPicker::pickPlanet(engine->camera, celestialBodies, scales);

      if (result.hit) {
        selectedPlanetIndex = static_cast<int>(result.planetIndex);
        std::cout << "Selected planet: "
                  << PlanetInfoPanel::getPlanetInfo(
                         celestialBodies[result.planetIndex].bodyType)
                         .name
                  << std::endl;
      } else {
        selectedPlanetIndex = -1;
        std::cout << "Deselected planet" << std::endl;
      }
    };

    engine->render(onRender);
  } catch (const std::exception& e) {
    std::cerr << "Application error: " << e.what() << std::endl;
    return -1;
  }
  return 0;
}

void onRender() {
  static float lastTime = 0.0f;
  frameCount++;

  float currentTime = static_cast<float>(glfwGetTime());
  float deltaTime = currentTime - lastTime;

  // Calculate FPS every 0.5 seconds for smooth display
  if (currentTime - lastFPSTime >= 0.5f) {
    currentFPS = frameCount / (currentTime - lastFPSTime);
    frameCount = 0;
    lastFPSTime = currentTime;
  }

  lastTime = currentTime;

  for (auto& body : celestialBodies) {
    body.updateOrbitalPositions(deltaTime * TIME_SCALE);
  }

  glm::mat4 view = engine->camera.getViewMatrix();
  glm::mat4 projection =
      glm::perspective(glm::radians(engine->camera.Zoom),
                       static_cast<float>(engine->SCR_WIDTH) /
                           static_cast<float>(engine->SCR_HEIGHT),
                       0.1f, 10000.0f);

  skybox->render(view, projection);

  for (size_t i = 0; i < celestialBodies.size(); i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, celestialBodies[i].position);

    float rotationSpeed = getPlanetsRotationSpeed(celestialBodies[i].bodyType);

    glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.1f);

    if (celestialBodies[i].bodyType == CelestialBody::Uranus) {
      rotationAxis = glm::vec3(0.8f, 0.2f, 0.1f);
    }

    model = glm::rotate(model, currentTime * glm::radians(rotationSpeed),
                        rotationAxis);

    glm::vec3 scale = getPlanetScale(celestialBodies[i].bodyType);
    model = glm::scale(model, scale);

    celestialBodies[i].render(model, view, projection);

    if (celestialBodies[i].bodyType == CelestialBody::Saturn) {
      saturnRing->render(model, view, projection);
    }
  }

  renderUI();
}

void renderUI() {
  if (!textRenderer) return;

  // Display FPS in top right corner
  std::string fpsText = "FPS: " + std::to_string(static_cast<int>(currentFPS));
  float fpsX = engine->SCR_WIDTH - 500.0f;
  float fpsY = 20.0f;
  textRenderer->renderText(fpsText, fpsX, fpsY, 3.0f,
                           glm::vec3(0.0f, 1.0f, 0.0f));

  // Display frame time (milliseconds per frame)
  if (currentFPS > 0.0f) {
    float frameTime = 1000.0f / currentFPS;
    std::string frameTimeText = std::to_string(static_cast<int>(frameTime)) + " ms";
    textRenderer->renderText(frameTimeText, fpsX, fpsY + 30.0f, 2.5f,
                             glm::vec3(0.8f, 0.8f, 0.8f));  // Light gray
  }

  textRenderer->renderText("CONTROLS:", 20.0f, 20.0f, 3.0f,
                           glm::vec3(1.0f, 1.0f, 0.0f));
  textRenderer->renderText("WASD    -   MOVE", 20.0f, 50.0f, 2.8f,
                           glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer->renderText("E       -   MOVE UP", 20.0f, 80.0f, 2.8f,
                           glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer->renderText("Q       -   MOVE DOWN", 20.0f, 110.0f, 2.8f,
                           glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer->renderText("SHIFT   -   SPRINT", 20.0f, 140.0f, 2.8f,
                           glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer->renderText("MOUSE   -   LOOK AROUND", 20.0f, 170.0f, 2.8f,
                           glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer->renderText("SCROLL  -   ZOOM", 20.0f, 200.0f, 2.8f,
                           glm::vec3(1.0f, 1.0f, 1.0f));
  textRenderer->renderText("CLICK   -   SELECT PLANET", 20.0f, 230.0f, 2.8f,
                           glm::vec3(0.5f, 1.0f, 0.5f));
  textRenderer->renderText("ESC     -   EXIT", 20.0f, 260.0f, 2.8f,
                           glm::vec3(1.0f, 0.5f, 0.5f));

  textRenderer->renderText("SOLAR SYSTEM SIMULATION",
                           (engine->SCR_WIDTH/2.0f) - 300.0f,
                           (engine->SCR_HEIGHT / 2.0f) - 400.0f, 3.0f,
                           glm::vec3(0.0f, 0.8f, 1.0f));

  glm::vec3 camPos = engine->camera.Position;
  std::string posText =
      "POSITION: X:" + std::to_string(static_cast<int>(camPos.x)) +
      " Y:" + std::to_string(static_cast<int>(camPos.y)) +
      " Z:" + std::to_string(static_cast<int>(camPos.z));
  textRenderer->renderText(posText, (engine->SCR_WIDTH / 2.0f) - 300.0f,
                           (engine->SCR_HEIGHT / 2.0f) - 350.0f, 3.0f,
                           glm::vec3(0.0f, 0.8f, 1.0f));

  // Render crosshair
  float centerX = engine->SCR_WIDTH / 2.0f;
  float centerY = engine->SCR_HEIGHT / 2.0f;
  textRenderer->renderText(":+:", centerX - 12.0f, centerY - 12.0f, 4.0f,
                           glm::vec3(0.0f, 1.0f, 0.0f));

  // *** ADD THIS CODE TO RENDER PLANET INFO PANEL ***
  if (planetInfoPanel && selectedPlanetIndex >= 0 &&
      selectedPlanetIndex < static_cast<int>(celestialBodies.size())) {
    const auto& selectedPlanet = celestialBodies[selectedPlanetIndex];
    PlanetInfo info = PlanetInfoPanel::getPlanetInfo(selectedPlanet.bodyType);

    planetInfoPanel->renderPanel(
        selectedPlanet.position,
        info,
        engine->camera,
        static_cast<float>(engine->SCR_WIDTH),
        static_cast<float>(engine->SCR_HEIGHT)
    );
  }
}

float getPlanetsRotationSpeed(CelestialBody::BodyType body) {
  float rotationSpeed;
  switch (body) {
    case CelestialBody::Sun: {
      rotationSpeed = 5.0f;
      break;
    }
    case CelestialBody::Mercury: {
      rotationSpeed = 15.0f;
      break;
    }
    case CelestialBody::Venus: {
      rotationSpeed = -8.0f;
      break;
    }
    case CelestialBody::Earth: {
      rotationSpeed = 50.0f;
      break;
    }
    case CelestialBody::Mars: {
      rotationSpeed = 48.0f;
      break;
    }
    case CelestialBody::Jupiter: {
      rotationSpeed = 120.0f;
      break;
    }
    case CelestialBody::Saturn: {
      rotationSpeed = 110.0f;
      break;
    }
    case CelestialBody::Uranus: {
      rotationSpeed = 70.0f;
      break;
    }
    case CelestialBody::Neptune: {
      rotationSpeed = 65.0f;
      break;
    }
    default: {
      std::cout << "ERROR: CelestialBody::BodyType provided is unknown: returning "
              "default rotation speed"
           << std::endl;
      rotationSpeed = 0.0f;
      break;
    }
  }
  return rotationSpeed;
}
glm::vec3 getPlanetScale(CelestialBody::BodyType body) {
  glm::vec3 scale;
  switch (body) {
    case CelestialBody::Sun: {
      scale = glm::vec3(3.0f, 3.0f, 3.0f);
      break;
    }
    case CelestialBody::Mercury: {
      scale = glm::vec3(0.3f, 0.3f, 0.3f);
      break;
    }
    case CelestialBody::Venus: {
      scale = glm::vec3(0.8f, 0.8f, 0.8f);
      break;
    }
    case CelestialBody::Earth: {
      scale = glm::vec3(0.8f, 0.8f, 0.8f);
      break;
    }
    case CelestialBody::Mars: {
      scale = glm::vec3(0.5f, 0.5f, 0.5f);
      break;
    }
    case CelestialBody::Jupiter: {
      scale = glm::vec3(2.2f, 2.2f, 2.2f);
      break;
    }
    case CelestialBody::Saturn: {
      scale = glm::vec3(1.8f, 1.8f, 1.8f);
      break;
    }
    case CelestialBody::Uranus: {
      scale = glm::vec3(1.0f, 1.0f, 1.0f);
      break;
    }
    case CelestialBody::Neptune: {
      scale = glm::vec3(1.0f, 1.0f, 1.0f);
      break;
    }
    default: {
      std::cout << "ERROR: CelestialBody::BodyType provided is unknown: returning "
              "default scale"
           << std::endl;
      scale = glm::vec3(1.0f, 1.0f, 1.0f);
      break;
    }
  }
  return scale;
}