//
// Created by Daniel on 05-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_ENGINECONTEXT_H
#define SOLAR_SYSTEM_OPENGL_ENGINECONTEXT_H
#include <deque>
#include <memory>

#include <AppConfig.h>

class WindowManager;
class Camera;
class InputManager;
class SceneRenderer;
class UIRenderer;
class SceneData;
class ISceneRenderable;

struct EngineContext {
  // Core system instances
  std::unique_ptr<WindowManager> WindowManager;
  std::unique_ptr<Camera> Camera;
  std::unique_ptr<InputManager> InputManager;
  // Rendering
  std::unique_ptr<SceneRenderer> SceneRenderer;
  std::unique_ptr<UIRenderer> UiRenderer;
  // Data
  std::unique_ptr<SceneData> SceneData;
  std::deque<std::unique_ptr<ISceneRenderable>> Renderables;

  AppConfig appConfig;
};

#endif  // SOLAR_SYSTEM_OPENGL_ENGINECONTEXT_H
