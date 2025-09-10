
#include "engine.h"
#include "shader.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <memory>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const std::string WINDOW_NAME = "Solar System Simulation";
const bool ENABLE_GL_DEPTH_TEST = true;

std::unique_ptr<Shader> shader;

void onRender();

int main()
{
    try {
        Engine engine(WINDOW_NAME, SCR_WIDTH, SCR_HEIGHT, ENABLE_GL_DEPTH_TEST);

        float vertices[] = {
            // Triangle vertices
            -0.5f, -0.5f, 0.0f,  // Bottom left
             0.5f, -0.5f, 0.0f,  // Bottom right
             0.0f,  0.5f, 0.0f   // Top center
        };

        /*glm::vec3 cubePositions[] = {
            glm::vec3(0.0f,  0.0f,  0.0f),
            glm::vec3(2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f,  2.0f, -2.5f),
            glm::vec3(1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };*/

        shader = std::make_unique<Shader>("shaders/object.vert", "shaders/object.frag");
        shader->use();

        BufferConfig buffersConfig;
        buffersConfig.useVBO = true;
        buffersConfig.useVAO = true;
        buffersConfig.useEBO = false;

        buffersConfig.vertexData = vertices;
        buffersConfig.vertexDataSize = sizeof(vertices);
        
        engine.setupBuffers(buffersConfig);

        BufferConfig vertexPointerConfig;
        vertexPointerConfig.vertexAttributePointerIndex = 0;
        vertexPointerConfig.vertexAttributePointerStride = 3;
        vertexPointerConfig.vertexAttributePointerOffset = 0;
        
        engine.setupVertexAttribPointer(vertexPointerConfig);

        engine.renderLoop(onRender);
    }
    catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

void onRender()
{
    if (shader) {
        shader->use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        shader->setVec3("ourColor", 1.0f, 0.5f, 0.2f);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}