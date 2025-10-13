//
// Created by Daniel on 04-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_WINDOWMANAGER_H
#define SOLAR_SYSTEM_OPENGL_WINDOWMANAGER_H
#include <functional>
#include <string>

#include "GLFW/glfw3.h"

class WindowManager {
public:
  WindowManager() = default;
  ~WindowManager();

  void create(int width, int height, const std::string& title,
              const std::function<void()>& onWindowCreated);
  void run(const std::function<void()>& onFrame = nullptr);
  void toggleFullscreen();
  GLFWwindow* getWindow() const;
  float getGLFWTime() const;
  void shutdown() const;
private:
  GLFWwindow* window;

  // Fullscreen state
  bool isFullscreen = false;
  int windowedWidth;
  int windowedHeight;
  int windowedPosX = 0;
  int windowedPosY = 0;

  float currentTime = 0.0f;
};
#endif  // SOLAR_SYSTEM_OPENGL_WINDOWMANAGER_H
