#ifndef ENGINE_H
#define ENGINE_H

#include <functional>

#include <core/EngineContext.h>

class Engine {
 public:
  struct FrameContext {
    bool shouldTerminate = false;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentTime = 0.0f;
  };

  Engine(const EngineContext& engineContext, bool enable_gl_depth_test);

  void run(std::function<void(FrameContext&)> frameCallback);

 private:
  const EngineContext& engineContext_;

  void render(float currentTime) const;

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
};

#endif