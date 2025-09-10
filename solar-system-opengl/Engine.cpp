#include "Engine.h"

Engine::Engine(std::string windowName, int windowWidth, int windowHeight)
    : windowName(windowName), windowWidth(windowWidth), windowHeight(windowHeight),
    window(nullptr), isInitialized(false) {

    try {
        initGLFW();
        window = createWindow(windowName, windowWidth, windowHeight);
        initGLAD();
        isInitialized = true;
        std::cout << "Engine initialized successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Engine initialization failed: " << e.what() << std::endl;
        isInitialized = false;
    }
}

Engine::~Engine() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    std::cout << "Engine destroyed" << std::endl;
}

void Engine::renderLoop(std::function<void()> renderCallback) {
    if (!isInitialized || !window) {
        std::cerr << "Engine not properly initialized. Cannot start render loop." << std::endl;
        return;
    }

    std::cout << "Starting render loop..." << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        if (renderCallback) {
            renderCallback();
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

BufferObjects Engine::setupBuffers(const BufferConfig& config) {
    BufferObjects buffers;

    if (config.useVAO) {
        glGenVertexArrays(1, &buffers.VAO);
        glBindVertexArray(buffers.VAO);
    }

    if (config.useVBO) {
        glGenBuffers(1, &buffers.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);
        if (config.vertexData && config.vertexDataSize > 0) {
            glBufferData(GL_ARRAY_BUFFER, config.vertexDataSize, config.vertexData, GL_STATIC_DRAW);
        }
    }

    if (config.useEBO) {
        glGenBuffers(1, &buffers.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.EBO);
        if (config.indicesData && config.indicesDataSize > 0) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, config.indicesDataSize, config.indicesData, GL_STATIC_DRAW);
        }
    }

    return buffers;
}

void Engine::setupVertexAttribPointer(BufferConfig config) 
{
    glVertexAttribPointer(
        config.vertexAttributePointerIndex,
        3,
        GL_FLOAT,
        GL_FALSE,
        config.vertexAttributePointerStride * sizeof(float),
        (void*)(config.vertexAttributePointerOffset * sizeof(float)));

    glEnableVertexAttribArray(config.vertexAttributePointerIndex);
}
void Engine::initGLFW() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    std::cout << "GLFW initialized" << std::endl;
}

void Engine::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    std::cout << "GLAD initialized" << std::endl;
}

GLFWwindow* Engine::createWindow(std::string name, int width, int height) {
    GLFWwindow* newWindow = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!newWindow) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(newWindow);
    std::cout << "Window created: " << name << " (" << width << "x" << height << ")" << std::endl;

    return newWindow;
}

void Engine::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}