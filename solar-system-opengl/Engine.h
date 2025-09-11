#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <functional>

#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
    int vertexAttributePointerSize = 0;
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

// TODO: buffers cleanup
//glDeleteVertexArrays(1, &VAO);
//glDeleteBuffers(1, &VBO);
class Engine {
public:
    Engine(std::string windowName, int windowWidth, int windowHeight, bool enable_gl_depth_test);
    ~Engine();

    void renderLoop(std::function<void(BufferObjects*, ObjectData*)> renderCallback = nullptr, BufferObjects* buffers = nullptr, ObjectData* objectData = nullptr);
    void setupVertexAttribPointer(BufferConfig config);
    BufferObjects setupBuffers(const BufferConfig& config);
    SphereData generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount);
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