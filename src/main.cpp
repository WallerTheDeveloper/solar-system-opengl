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

// settings
const unsigned int SCR_WIDTH = 1980;
const unsigned int SCR_HEIGHT = 1080;
const std::string WINDOW_NAME = "Solar System Simulation";
const bool ENABLE_GL_DEPTH_TEST = true;

std::unique_ptr<Skybox> skybox;
std::vector<Planet> celestialBodies;

void onRender(Engine* engine);
float getPlanetsRotationSpeed(CelestialBody::BodyType body);
glm::vec3 getPlanetScale(CelestialBody::BodyType body);

int main() {
  try {
    Engine engine(WINDOW_NAME, SCR_WIDTH, SCR_HEIGHT, ENABLE_GL_DEPTH_TEST);

    skybox = make_unique<Skybox>(&engine, "../textures/skybox.jpg");

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
  float currentTime = static_cast<float>(glfwGetTime());
  float deltaTime = currentTime - lastTime;
  lastTime = currentTime;

  const float TIME_SCALE = 5.0f;

  for (auto& body : celestialBodies) {
    body.updateOrbitalPositions(deltaTime * TIME_SCALE);
  }

  glm::mat4 view = engine->camera.getViewMatrix();
  glm::mat4 projection = glm::perspective(
      glm::radians(engine->camera.Zoom),
      static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f,
      10000.0f);

  for (size_t i = 0; i < celestialBodies.size(); i++) {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, celestialBodies[i].position);

    float rotationSpeed = getPlanetsRotationSpeed(celestialBodies[i].bodyType);

    glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.1f);
    if (celestialBodies[i].bodyType == CelestialBody::Uranus) {
      rotationAxis = glm::vec3(0.8f, 0.2f, 0.1f);  // Uranus rotates on its side
    }
    model = glm::rotate(model, currentTime * glm::radians(rotationSpeed),
                        rotationAxis);

    glm::vec3 scale = getPlanetScale(celestialBodies[i].bodyType);

    model = glm::scale(model, scale);

    celestialBodies[i].render(model, view, projection);
  }

  // Render skybox last so that it appears behind everything
  skybox->render(view, projection);
}

float getPlanetsRotationSpeed(CelestialBody::BodyType body) {
  switch (body) {
    case CelestialBody::Sun: {
      return 5.0f;
    }
    case CelestialBody::Mercury: {
      return 15.0f;
    }
    case CelestialBody::Venus: {
      return -8.0f;
    }
    case CelestialBody::Earth: {
      return 50.0f;
    }
    case CelestialBody::Mars: {
      return 48.0f;
    }
    case CelestialBody::Jupiter: {
      return 120.0f;
    }
    case CelestialBody::Saturn: {
      return 110.0f;
    }
    case CelestialBody::Uranus: {
      return 70.0f;
    }
    case CelestialBody::Neptune: {
      return 65.0f;
    }
    default: {
      cout << "ERROR: CelestialBody::BodyType provided is unknown" << endl;
      return 0.0f;
    }
  }
}
glm::vec3 getPlanetScale(CelestialBody::BodyType body) {
  switch (body) {
    case CelestialBody::Sun: {
      return glm::vec3(3.0f, 3.0f, 3.0f);
    }
    case CelestialBody::Mercury: {
      return glm::vec3(0.3f, 0.3f, 0.3f);
    }
    case CelestialBody::Venus: {
      return glm::vec3(0.8f, 0.8f, 0.8f);
    }
    case CelestialBody::Earth: {
      return glm::vec3(0.8f, 0.8f, 0.8f);
    }
    case CelestialBody::Mars: {
      return glm::vec3(0.5f, 0.5f, 0.5f);
    }
    case CelestialBody::Jupiter: {
      return glm::vec3(2.2f, 2.2f, 2.2f);
    }
    case CelestialBody::Saturn: {
      return glm::vec3(1.8f, 1.8f, 1.8f);
    }
    case CelestialBody::Uranus: {
      return glm::vec3(1.0f, 1.0f, 1.0f);
    }
    case CelestialBody::Neptune: {
      return glm::vec3(1.0f, 1.0f, 1.0f);
    }
    default: {
      cout << "ERROR: CelestialBody::BodyType provided is unknown" << endl;
      return glm::vec3(0.0f, 0.0f, 0.0f);
    }
  }
}