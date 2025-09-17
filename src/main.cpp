#include "../headers/engine.h"
#include "../headers/planet.h"
#include "../headers/skybox.h"
#include "../headers/shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <string>


// settings
const unsigned int SCR_WIDTH = 1980;
const unsigned int SCR_HEIGHT = 1080;
const std::string WINDOW_NAME = "Solar System Simulation";
const bool ENABLE_GL_DEPTH_TEST = true;

std::unique_ptr<Skybox> skybox;

std::vector<CelestialBody> celestialBodies;

void onRender(Engine* engine);

int main()
{
    try {
        Engine engine(WINDOW_NAME, SCR_WIDTH, SCR_HEIGHT, ENABLE_GL_DEPTH_TEST);

        //objectShader = std::make_unique<Shader>("shaders/object.vert", "shaders/object.frag");
        //texture = std::make_unique<Texture>();
        Planet sun(&engine, "Sun", 1.989e30f, 696000000.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3(0.0f, 0.0f, 0.0f));

        sun.create("textures/sun.jpg");

        // celestialBodies.push_back(sun);

        /*celestialBodies.push_back(CelestialBody("Sun", 1.989e30f, 696000000.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
        celestialBodies.push_back(CelestialBody("Mercury", 3.301e23f, 2439700.0f, 57.9e9f, 0.205f, 87.97f * 24 * 3600, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)));
        celestialBodies.push_back(CelestialBody("Venus", 4.867e24f, 6051800.0f, 108.2e9f, 0.007f, 224.7f * 24 * 3600, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)));
        celestialBodies.push_back(CelestialBody("Earth", 5.972e24f, 6371000.0f, 149.6e9f, 0.017f, 365.25f * 24 * 3600, 0.0f,
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));*/


        /*skybox = std::make_unique<Skybox>("textures/skybox.jpg");
        if (!skybox->initialize(&engine)) {
          std::cerr << "Failed to initialize skybox" << std::endl;
          return -1;
        }*/

        engine.render(onRender, &engine);
    }
    catch (const std::exception& e) {
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

    const float TIME_SCALE = 1000000.0f/2.0f;
    const float SCALE_FACTOR = 1e-10f;  


    for (auto& body : celestialBodies) {
        body.updateOrbitalPositions(deltaTime * TIME_SCALE);
        if (body.name != "Sun") {
            body.position *= SCALE_FACTOR;
        }
    }

    glm::mat4 view = engine->camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(engine->camera.Zoom),
        (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);

    //// RENDER SKYBOX FIRST (before other objects)
    //if (skybox) {
    //    skybox->render(view, projection);
    //}


    //texture->setTextureActive2D(GL_TEXTURE0, objectTextureID);

    for (size_t i = 0; i < celestialBodies.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, celestialBodies[i].position);
        model = glm::rotate(model, currentTime * glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}