#ifndef ENGINE_H
#define ENGINE_H

#include <core/EngineContext.h>
#include <rendering/renderers/TextRenderer.h>

#include <deque>
#include <functional>

class ISceneRenderable;

class Engine {
 public:
  struct FrameContext {
    bool shouldTerminate = false;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentTime = 0.0f;
  };

  Engine(bool enable_gl_depth_test, BufferManager& bufferManager);
  ~Engine();

  void run(std::function<void(FrameContext&)> frameCallback,
           const std::deque<std::unique_ptr<ISceneRenderable>>& renderables);

 private:
  std::unique_ptr<EngineContext> context_;
  BufferManager& bufferManager_;

  void render(
      float currentTime,
      const std::deque<std::unique_ptr<ISceneRenderable>>& renderables) const;

  // Time & performance tracking
  float lastFPSTime_ = 0.0f;
  float currentFPS_ = 0.0f;
  int frameCount_ = 0;

  // Helpers
  void calculateFPS(float currentTime);

  bool isInitialized;

  bool enableCullFaceState = false;

  // Core system functions
  void initGLFW();
  void initGLAD();

  // Input
  void setupInputConfig() const;
};

#endif