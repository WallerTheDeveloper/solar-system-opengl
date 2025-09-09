#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <functional>

class Engine {
public:
    Engine(std::string windowName, int windowWidth, int windowHeight);
    ~Engine();
    void renderLoop(std::function<void()> renderCallback = nullptr);

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