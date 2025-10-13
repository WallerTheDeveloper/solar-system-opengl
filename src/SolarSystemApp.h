//
// Created by Daniel on 30-Sep-25.
//

#ifndef SOLAR_SYSTEM_APP_H
#define SOLAR_SYSTEM_APP_H


#include <deque>
#include <memory>

#include <rendering/renderables/scene/ISceneRenderable.h>

class WindowManager;
class InputManager;
class Camera;
class SceneRenderer;
class UIRenderer;
class Engine;
class BufferManager;
class Skybox;
class Ring;
class TextRenderer;
class CelestialBodyInfoPanel;
class MeshGenerator;
class TextureManager;

class SolarSystemApp {
 public:
  SolarSystemApp();
  ~SolarSystemApp();

  SolarSystemApp(const SolarSystemApp&) = delete;
  SolarSystemApp& operator=(const SolarSystemApp&) = delete;

  bool initialize();
  void run();
  void shutdown();

 private:
  std::unique_ptr<Engine> engine_;
  std::unique_ptr<BufferManager> bufferManager_;
  std::unique_ptr<Skybox> skybox_;
  std::unique_ptr<Ring> saturnRing_;
  std::unique_ptr<MeshGenerator> meshGenerator_;
  std::unique_ptr<TextureManager> textureManager_;

  std::deque<std::unique_ptr<ISceneRenderable>> renderables_;
  // std::vector<std::unique_ptr<CelestialBody>> celestialBodies_;
  // int selectedPlanetIndex_;

  bool initializePlanets(BufferManager& bufferManager,
                         TextureManager& textureManager,
                         MeshGenerator& meshGenerator);
  // Helpers
  void handlePlanetSelection(const Camera& camera);
};

#endif  // SOLAR_SYSTEM_APP_H