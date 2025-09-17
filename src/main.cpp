#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "../headers/engine.h"
#include "../headers/planet.h"
#include "../headers/shader.h"
#include "../headers/skybox.h"

// settings
const unsigned int SCR_WIDTH = 1980;
const unsigned int SCR_HEIGHT = 1080;
const std::string WINDOW_NAME = "Solar System Simulation";
const bool ENABLE_GL_DEPTH_TEST = true;

std::unique_ptr<Skybox> skybox;

std::vector<Planet> celestialBodies;

void onRender(Engine* engine);

int main() {
  try {
    Engine engine(WINDOW_NAME, SCR_WIDTH, SCR_HEIGHT, ENABLE_GL_DEPTH_TEST);

    celestialBodies.emplace_back(&engine, "Sun", 1.989e30f, 696000000.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f, glm::vec3(5.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));

    celestialBodies[0].create("../textures/sun.jpg");

    celestialBodies.emplace_back(&engine, "Mercury", 3.301e23f, 2439700.0f,
                                 5.0f, 0.205f, 87.97f * 24 * 3600, 0.0f,
                                 glm::vec3(5.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f));
    celestialBodies[1].create("../textures/mercury.jpg");

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

  const float TIME_SCALE = 1000000.0f / 2.0f;
  const float SCALE_FACTOR = 1e-10f;

  for (auto& body : celestialBodies) {
    body.updateOrbitalPositions(deltaTime * TIME_SCALE);
    if (body.name != "Sun") {
      body.position *= SCALE_FACTOR;
    }
  }

  glm::mat4 view = engine->camera.getViewMatrix();
  glm::mat4 projection =
      glm::perspective(glm::radians(engine->camera.Zoom),
                       static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 10000.0f);

  //// RENDER SKYBOX FIRST (before other objects)
  // if (skybox) {
  //     skybox->render(view, projection);
  // }

  for (size_t i = 0; i < celestialBodies.size(); i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, celestialBodies[i].position);
    model = glm::rotate(model, currentTime * glm::radians(30.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));

    celestialBodies[i].render(model, view, projection);
  }
}