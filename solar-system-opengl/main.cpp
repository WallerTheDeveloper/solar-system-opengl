
#include "engine.h"
#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


#include <iostream>
#include <string>
#include <memory>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const std::string WINDOW_NAME = "Solar System Simulation";
const bool ENABLE_GL_DEPTH_TEST = true;

unsigned int objectTextureID;

std::unique_ptr<Shader> shader;
std::unique_ptr<Texture> texture;

void onRender(BufferObjects* buffers, ObjectData* objectData, Engine* engine);

int main()
{
    try {
        Engine engine(WINDOW_NAME, SCR_WIDTH, SCR_HEIGHT, ENABLE_GL_DEPTH_TEST);

        SphereData sphereData = engine.generateSphere(0.5f, 36, 18);
        shader = std::make_unique<Shader>("shaders/object.vert", "shaders/object.frag");
        texture = std::make_unique<Texture>();

        BufferConfig sphereDataConfig;
        sphereDataConfig.useVBO = true;
        sphereDataConfig.useEBO = true;
        sphereDataConfig.useVAO = true;
        sphereDataConfig.vertexData = sphereData.vertices.data();
        sphereDataConfig.vertexDataSize = sphereData.vertices.size() * sizeof(float);
        sphereDataConfig.indicesData = sphereData.indices.data();
        sphereDataConfig.indicesDataSize = sphereData.indices.size() * sizeof(unsigned int);
        sphereDataConfig.vertexAttributePointerIndex = 0;
        sphereDataConfig.vertexAttributePointerSize = 3;
        sphereDataConfig.vertexAttributePointerStride = 5;
        sphereDataConfig.vertexAttributePointerOffset = 0;

        BufferObjects sphereBuffers = engine.setupBuffers(sphereDataConfig);
        engine.setupVertexAttribPointer(sphereDataConfig);

        BufferConfig textureDataConfig;
        textureDataConfig.vertexAttributePointerIndex = 1;
        textureDataConfig.vertexAttributePointerSize = 2;
        textureDataConfig.vertexAttributePointerStride = 5;
        textureDataConfig.vertexAttributePointerOffset = 3;

        engine.setupVertexAttribPointer(textureDataConfig);

        texture->generateTexture(1, objectTextureID);

        int width = 0, height, numberOfChannels;
        const char* filename = "textures/sun.jpg";

        unsigned char* data =  texture->loadTextureImage(filename, width, height, numberOfChannels);

        if (data)
        {
            texture->specifyTextureImage2D(data, width, height, true);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        texture->freeImageData(data);

        shader->setInt("texture", 0);

        engine.renderLoop(onRender, &sphereBuffers, &sphereData, &engine);
    }
    catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
void onRender(BufferObjects* buffers, ObjectData* objectData, Engine* engine)
{
    if (shader && buffers && objectTextureID) {

        texture->setTextureActive2D(GL_TEXTURE0, objectTextureID);

        shader->use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(0.85f, 0.85f, 0.0f));
        
        glm::mat4 view = engine->camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(engine->camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        shader->setVec3("ourColor", 1.0f, 1.0f, 0.0f);


        glBindVertexArray(buffers->VAO);
        glDrawElements(GL_TRIANGLES, objectData->indicesCount, GL_UNSIGNED_INT, 0);
    }
}