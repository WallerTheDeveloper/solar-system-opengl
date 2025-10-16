//
// Created by Daniel on 06-Oct-25.
//

#include "InputManager.h"

InputManager::InputManager(int windowWidth,
                           int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {}

void InputManager::setInputCallbacks(GLFWwindow* window) const {
  glfwSetWindowUserPointer(window, const_cast<InputManager*>(this));

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, pointer_position_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetMouseButtonCallback(window, button_callback);
  glfwSetKeyCallback(window, key_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool InputManager::processInput(GLFWwindow* window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return true;
  }

  float speedMultiplier = 1.0f;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    speedMultiplier = 5.0f;
  }

  for (auto& [key, action] : keyBindings) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
      action(deltaTime, speedMultiplier);
    }
  }
  return false;
}

void InputManager::bindKey(int key, const std::function<void(float deltaTime, float speedMultiplier)>&
                 onKeyBind) {
  keyBindings[key] = onKeyBind;
}

void InputManager::setPointerMovementCallback(
    const PointerMovementCallback& callback) {
  pointerMovementCallback_ = callback;
}

void InputManager::setAxisCallback(const AxisCallback& callback) {
  scrollCallback_ = callback;
}

void InputManager::setPrimaryActionCallback(
    const PrimaryActionCallback& callback) {
  primaryActionCallback_ = callback;
}

void InputManager::setFullscreenActionCallback(
    const FullscreenActionCallback& callback) {
  fullscreenActionCallback_ = callback;
}

void InputManager::framebuffer_size_callback(GLFWwindow* window, int width,
                                             int height) {
  glViewport(0, 0, width, height);
}

void InputManager::pointer_position_callback(GLFWwindow* window, double xposIn, double yposIn) {
  auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
  if (manager) {
    manager->handlePointerMovement(xposIn, yposIn);
  }
}

void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
  if (manager) {
    manager->handleAxis(yoffset);
  }
}

void InputManager::button_callback(GLFWwindow* window, int button, int action, int mods) {
  auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
  if (manager) {
    manager->handlePrimaryActionKey(button, action);
  }
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
  if (manager) {
    manager->handleFullscreenKey(window, key, scancode, action, mods);
  }
}

void InputManager::handlePointerMovement(double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstPointer_) {
    lastPointerX_ = xpos;
    lastPointerY_ = ypos;
    firstPointer_ = false;
  }

  float xoffset = xpos - lastPointerX_;
  float yoffset = lastPointerY_ - ypos;

  lastPointerX_ = xpos;
  lastPointerY_ = ypos;

  if (pointerMovementCallback_) {
    pointerMovementCallback_(xoffset, yoffset);
  }
}

void InputManager::handleAxis(double yoffset) const {
  if (scrollCallback_) {
    scrollCallback_(static_cast<float>(yoffset));
  }
}

void InputManager::handlePrimaryActionKey(int button, int action) const {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (primaryActionCallback_) {
      primaryActionCallback_();
    }
  }
}

void InputManager::handleFullscreenKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
  // ENTER + SHIFT = fullscreen mode
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && (mods & GLFW_MOD_SHIFT)) {
    fullscreenActionCallback_();
  }
}