
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

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

std::unique_ptr<Shader> shader;

void onRender(BufferObjects* buffers, ObjectData* objectData);

int main()
{
    try {
        Engine engine(WINDOW_NAME, SCR_WIDTH, SCR_HEIGHT, ENABLE_GL_DEPTH_TEST);

        SphereData sphereData = engine.generateSphere(0.5f, 36, 18);
        shader = std::make_unique<Shader>("shaders/object.vert", "shaders/object.frag");

        BufferConfig sphereConfig;
        sphereConfig.useVBO = true;
        sphereConfig.useEBO = true;
        sphereConfig.useVAO = true;
        sphereConfig.vertexData = sphereData.vertices.data();
        sphereConfig.vertexDataSize = sphereData.vertices.size() * sizeof(float);
        sphereConfig.indicesData = sphereData.indices.data();
        sphereConfig.indicesDataSize = sphereData.indices.size() * sizeof(unsigned int);
        sphereConfig.vertexAttributePointerIndex = 0;
        sphereConfig.vertexAttributePointerStride = 3;
        sphereConfig.vertexAttributePointerOffset = 0;

        BufferObjects sphereBuffers = engine.setupBuffers(sphereConfig);
        engine.setupVertexAttribPointer(sphereConfig);

        engine.renderLoop(onRender, &sphereBuffers, &sphereData);
    }
    catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
void onRender(BufferObjects* buffers, ObjectData* objectData)
{
    if (shader && buffers) {
        shader->use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        shader->setVec3("ourColor", 1.0f, 1.0f, 0.0f);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBindVertexArray(buffers->VAO);
        glDrawElements(GL_TRIANGLES, objectData->indicesCount, GL_UNSIGNED_INT, 0);
    }
}