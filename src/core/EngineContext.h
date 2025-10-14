//
// Created by Daniel on 05-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_ENGINECONTEXT_H
#define SOLAR_SYSTEM_OPENGL_ENGINECONTEXT_H
#include <memory>

class WindowManager;
class Camera;
class InputManager;
class SceneRenderer;
class UIRenderer;
class TextRenderer;
class CelestialBodyInfoPanel;
class AudioManager;

struct EngineContext {
  // Core system instances
  std::unique_ptr<WindowManager> windowManager;
  std::unique_ptr<Camera> camera;
  std::unique_ptr<InputManager> inputManager;
  // Rendering
  std::unique_ptr<SceneRenderer> sceneRenderer;
  std::unique_ptr<UIRenderer> uiRenderer;
  std::unique_ptr<TextRenderer> textRenderer;
  // UI
  std::unique_ptr<CelestialBodyInfoPanel> bodyInfoPanel;
  // Audio
  std::unique_ptr<AudioManager> audioManager;
};

#endif  // SOLAR_SYSTEM_OPENGL_ENGINECONTEXT_H
