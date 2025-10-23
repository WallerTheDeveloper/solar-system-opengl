//
// Created by Daniel on 04-Oct-25.
//

#include "WindowManager.h"

#include <iostream>
#include <stdexcept>

WindowManager::~WindowManager() {
  shutdown();
}

void WindowManager::create(int width, int height, const std::string& title,
                           const std::function<void()>& onWindowCreated) {
  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(window);
  std::cout << "Window created: " << title << " (" << width << "x" << height
            << ")" << std::endl;

  this->windowedWidth = width;
  this->windowedHeight = height;

  isTerminated = false;

  if (onWindowCreated) {
    onWindowCreated();
  }
}

void WindowManager::run(const std::function<void()>& onFrame) {
  while (!glfwWindowShouldClose(window)) {
    if (isTerminated) {
      glfwSetWindowShouldClose(window, true);
      return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    currentTime = static_cast<float>(glfwGetTime());

    if (onFrame) {
      onFrame();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void WindowManager::toggleFullscreen() {
  if (!window) {
    return;
  }

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  if (!isFullscreen) {
    glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
    glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

    std::cout << "Switched to fullscreen mode (" << mode->width << "x" << mode->height << ")" << std::endl;
    isFullscreen = true;
  } else {
    glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);

    std::cout << "Switched to windowed mode (" << windowedWidth << "x" << windowedHeight << ")" << std::endl;
    isFullscreen = false;
  }
}

GLFWwindow* WindowManager::getWindow() const {
  return window;
}

float WindowManager::getGLFWTime() const {
  return currentTime;
}

void WindowManager::shutdown() {
  if (window) {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
  isTerminated = true;
  std::cout << "Window manager cleaned up" << std::endl;
}