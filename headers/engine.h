#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "camera.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct BufferObjects {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
};

struct ObjectMeshData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int indicesCount;
};

struct SphereMeshData : ObjectMeshData {
    
};


class Engine {
public:
    Camera camera;
    float deltaTime = 0.0f;  // time between current frame and last frame
    float lastFrame = 0.0f;

    static Engine& getInstance();
    static bool isEngineInitialized();

    Engine(std::string windowName, int windowWidth, int windowHeight, bool enable_gl_depth_test);
    ~Engine();

    void render(std::function<void(Engine*)> renderCallback = nullptr,
                Engine* engine = nullptr);

    // Buffers
    void generateVAO(unsigned int* VAO);
    void generateBuffer(unsigned int* buffer);
    
    void bindVAO(unsigned int VAO);
    void bindBuffer(GLenum target, unsigned int buffer);

    void setBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
    void defineVertexLayout(unsigned int shaderLayoutIndex, int size,
                            GLenum type, GLboolean normalizeData, int stride,
                            const void* offset); 

    void deleteVAO(int size, unsigned int& VAO);
    void deleteBuffers(int size, const unsigned int& buffer);

    // Primitives mesh generation
    SphereMeshData generateSphereMesh(float radius, unsigned int sectorCount, unsigned int stackCount);

private:
    static Engine* instance;
    GLFWwindow* window;
    std::string windowName;
    int windowWidth;
    int windowHeight;
    bool isInitialized;
    float lastMouseX = (float)windowWidth / 2.0f;
    float lastMouseY = (float)windowHeight / 2.0f;
    bool firstMouse = true;

    void initGLFW();
    void initGLAD();
    void processInput(GLFWwindow* window);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    GLFWwindow* createWindow(std::string name, int width, int height);
};

#endif