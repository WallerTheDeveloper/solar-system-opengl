#include "Engine.h"

Engine::Engine(std::string windowName, int windowWidth, int windowHeight, bool enable_gl_depth_test)
    : windowName(windowName), windowWidth(windowWidth), windowHeight(windowHeight),
    window(nullptr), isInitialized(false) {

    try {
        initGLFW();
        window = createWindow(windowName, windowWidth, windowHeight);
        initGLAD();

        if (enable_gl_depth_test)
        {
            glEnable(GL_DEPTH_TEST);
        }

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

void Engine::renderLoop(std::function<void(BufferObjects*, ObjectData*)> renderCallback, BufferObjects* buffers, ObjectData* objectData) {
    if (!isInitialized || !window) {
        std::cerr << "Engine not properly initialized. Cannot start render loop." << std::endl;
        return;
    }

    std::cout << "Starting render loop..." << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        if (renderCallback && buffers) {
            renderCallback(buffers, objectData);
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

SphereData Engine::generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount) {
    SphereData sphere;

    float x, y, z, xy;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    // Generate vertices
    for (unsigned int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            // Position
            sphere.vertices.push_back(x);
            sphere.vertices.push_back(y);
            sphere.vertices.push_back(z);

            // Texture coordinates
            float u = (float)j / sectorCount;
            float v = (float)i / stackCount;
            sphere.vertices.push_back(u);
            sphere.vertices.push_back(v);
        }
    }

    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) 
        {
            if (i != 0) 
            {
                sphere.indices.push_back(k1);
                sphere.indices.push_back(k2);
                sphere.indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) 
            {
                sphere.indices.push_back(k1 + 1);
                sphere.indices.push_back(k2);
                sphere.indices.push_back(k2 + 1);
            }
        }
    }

    sphere.indicesCount = sphere.indices.size();

    return sphere;
}

void Engine::setupVertexAttribPointer(BufferConfig config) 
{
    glVertexAttribPointer(
        config.vertexAttributePointerIndex,
        config.vertexAttributePointerSize,
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

void Engine::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* Engine::createWindow(std::string name, int width, int height) 
{
    GLFWwindow* newWindow = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!newWindow) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(newWindow);
    std::cout << "Window created: " << name << " (" << width << "x" << height << ")" << std::endl;

    return newWindow;
}
