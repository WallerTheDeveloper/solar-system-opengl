#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

#include "../headers/engine.h"
#include "../headers/planet.h"
#include "../headers/shader.h"
#include "../headers/skybox.h"
#include "celestialbody.h"
#include "ring.h"
#include "textrenderer.h"

const std::string WINDOW_NAME = "Solar System Simulation";
const bool ENABLE_GL_DEPTH_TEST = true;

std::unique_ptr<Skybox> skybox;
std::unique_ptr<Ring> saturnRing;
std::unique_ptr<TextRenderer> textRenderer;

std::vector<Planet> celestialBodies;

void onRender(Engine* engine);
void renderUI(Engine* engine);

float getPlanetsRotationSpeed(CelestialBody::BodyType body);
glm::vec3 getPlanetScale(CelestialBody::BodyType body);

int main() {
  try {
    Engine engine(WINDOW_NAME, ENABLE_GL_DEPTH_TEST);

    vector<std::string> faces
    {
      "../textures/skybox1.png",
      "../textures/skybox2.png",
      "../textures/skybox3.png",
      "../textures/skybox4.png",
      "../textures/skybox5.png",
      "../textures/skybox6.png"
    };


    skybox = make_unique<Skybox>(&engine, faces);
    saturnRing = make_unique<Ring>();
    saturnRing->create(&engine, "../textures/saturn_ring.png");

    textRenderer = std::make_unique<TextRenderer>(&engine);
    if (!textRenderer->initialize()) {
      std::cerr << "Failed to initialize text renderer!" << std::endl;
      return -1;
    }

    celestialBodies.emplace_back(
        &engine, CelestialBody::Sun, 1.989e30f, 696340000.0f, 0.0f, 0.0f, 0.0f,
        0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[0].create("../textures/sun.jpg");

    celestialBodies.emplace_back(
        &engine, CelestialBody::Mercury, 3.301e23f, 2439700.0f, 4.0f, 0.205f,
        20.0f, 0.0f, glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[1].create("../textures/mercury.jpg");

    celestialBodies.emplace_back(
        &engine, CelestialBody::Venus, 4.867e24f, 6051800.0f, 6.0f, 0.007f,
        35.0f, 0.0f, glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[2].create("../textures/venus.jpg");

    celestialBodies.emplace_back(
        &engine, CelestialBody::Earth, 5.972e24f, 6371000.0f, 8.0f, 0.017f,
        50.0f, 0.0f, glm::vec3(8.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[3].create("../textures/earth.jpg");

    celestialBodies.emplace_back(
        &engine, CelestialBody::Mars, 6.417e23f, 3389500.0f, 10.5f, 0.094f,
        75.0f, 0.0f, glm::vec3(10.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[4].create("../textures/mars.jpg");

    celestialBodies.emplace_back(&engine, CelestialBody::Jupiter, 1.898e27f,
                                 69911000.0f, 16.0f, 0.049f, 120.0f, 0.0f,
                                 glm::vec3(16.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[5].create("../textures/jupiter.jpg");

    celestialBodies.emplace_back(&engine, CelestialBody::Saturn, 5.683e26f,
                                 58232000.0f, 20.0f, 0.057f, 180.0f, 0.0f,
                                 glm::vec3(20.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[6].create("../textures/saturn.jpg");

    celestialBodies.emplace_back(&engine, CelestialBody::Uranus, 8.681e25f,
                                 25362000.0f, 25.0f, 0.046f, 250.0f, 0.0f,
                                 glm::vec3(25.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[7].create("../textures/uranus.jpg");

    celestialBodies.emplace_back(&engine, CelestialBody::Neptune, 1.024e26f,
                                 24622000.0f, 30.0f, 0.009f, 350.0f, 0.0f,
                                 glm::vec3(30.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[8].create("../textures/neptune.jpg");

    if (!skybox->create()) {
      std::cerr << "Failed to create skybox!" << std::endl;
      return -1;
    }

    engine.render(onRender, &engine);
  } catch (const std::exception& e) {
    std::cerr << "Application error: " << e.what() << std::endl;
    return -1;
  }
  return 0;
}

void onRender(Engine* engine) {
  static float lastTime = 0.0f;
  static int frameCount = 0;
  frameCount++;

  float currentTime = static_cast<float>(glfwGetTime());
  float deltaTime = currentTime - lastTime;
  lastTime = currentTime;

  const float TIME_SCALE = 5.0f;

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

  renderUI(engine);
}

void renderUI(Engine* engine) {
  if (!textRenderer) return;

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
  textRenderer->renderText("ESC     -   EXIT", 20.0f, 230.0f, 2.8f,
                           glm::vec3(1.0f, 0.5f, 0.5f));


  textRenderer->renderText("SOLAR SYSTEM SIMULATION", (engine->SCR_WIDTH/2.0f) - 300.0f,
                           (engine->SCR_HEIGHT / 2.0f) - 400.0f, 3.0f,
                           glm::vec3(0.0f, 0.8f, 1.0f));

  glm::vec3 camPos = engine->camera.Position;
  std::string posText = "POSITION:(X:" + std::to_string(static_cast<int>(camPos.x)) + "." +
                        std::to_string(static_cast<int>(camPos.y)) + ", Y:" +
                        std::to_string(static_cast<int>(camPos.z)) + ")";
  textRenderer->renderText(posText, (engine->SCR_WIDTH/2.0f) - 300.0f,
                           (engine->SCR_HEIGHT / 2.0f) - 350.0f, 3.0f,
                           glm::vec3(0.0f, 0.8f, 1.0f));
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
      cout << "ERROR: CelestialBody::BodyType provided is unknown: returning "
              "default rotation speed"
           << endl;
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
      cout << "ERROR: CelestialBody::BodyType provided is unknown: returning "
              "default scale"
           << endl;
      scale = glm::vec3(1.0f, 1.0f, 1.0f);
      break;
    }
  }
  return scale;
}