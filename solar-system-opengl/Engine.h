#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <functional>
#include <stb_image/stb_image.h>

struct BufferObjects {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
};

struct BufferConfig {
    bool useVBO = true;
    bool useVAO = false;
    bool useEBO = false;

    const void* vertexData = nullptr;
    size_t vertexDataSize = 0;

    const void* indicesData = nullptr;
    size_t indicesDataSize = 0;

    int vertexAttributePointerIndex = 0;
    int vertexAttributePointerStride = 0;
    int vertexAttributePointerOffset = 0;
};

struct ObjectData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int indicesCount;
};

struct SphereData : ObjectData {
    
};
class Engine {
public:
    Engine(std::string windowName, int windowWidth, int windowHeight, bool enable_gl_depth_test);
    ~Engine();
    void renderLoop(std::function<void()> renderCallback = nullptr);
    BufferObjects setupBuffers(const BufferConfig& config);
    void setupVertexAttribPointer(BufferConfig config);
private:
    GLFWwindow* window;
    std::string windowName;
    int windowWidth;
    int windowHeight;
    bool isInitialized;

    void initGLFW();
    void initGLAD();
    void processInput(GLFWwindow* window);
    GLFWwindow* createWindow(std::string name, int width, int height);
};

#endif