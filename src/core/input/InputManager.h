//
// Created by Daniel on 06-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_INPUTMANAGER_H
#define SOLAR_SYSTEM_OPENGL_INPUTMANAGER_H

#include "GLFW/glfw3.h"
#include <functional>

class InputManager {
public:
  using PointerMovementCallback = std::function<void(float xoffset, float yoffset)>;
  using AxisCallback = std::function<void(float value)>;
  using PrimaryActionCallback = std::function<void()>;
  using KeyActionCallback = std::function<void(float deltaTime, float speedMultiplier)>;

  InputManager(int windowWidth, int windowHeight);
  void setInputCallbacks(GLFWwindow* window) const;
  bool processInput(GLFWwindow* window, float deltaTime);
  void bindKey(int key,
               const std::function<void(float deltaTime, float speedMultiplier)>& onKeyBind);

  void setPointerMovementCallback(PointerMovementCallback callback);
  void setAxisCallback(AxisCallback callback);
  void setPrimaryActionCallback(PrimaryActionCallback callback);

private:
  int windowWidth_;
  int windowHeight_;

  float lastPointerX_ = static_cast<float>(windowWidth_) / 2.0f;;
  float lastPointerY_ = static_cast<float>(windowHeight_) / 2.0f;;
  bool firstPointer_ = true;

  std::function<void(float deltaTime, float speedMultiplier)> onKeyBind_;

  std::unordered_map<int, std::function<void(float deltaTime, float speedMultiplier)>> keyBindings;

  PointerMovementCallback pointerMovementCallback_;
  AxisCallback scrollCallback_;
  PrimaryActionCallback primaryActionCallback_;

  // Input callbacks
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  static void pointer_position_callback(GLFWwindow* window, double xposIn, double yposIn);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  static void button_callback(GLFWwindow* window, int button, int action, int mods);
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  void handlePointerMovement(double xposIn, double yposIn);
  void handleAxis(double yoffset);
  void handlePrimaryActionKey(int button, int action);
  void handleFullscreenKey(int key, int scancode, int action, int mods);
};
#endif  // SOLAR_SYSTEM_OPENGL_INPUTMANAGER_H
