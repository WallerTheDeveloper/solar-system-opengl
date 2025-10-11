//
// Created by Daniel on 30-Sep-25.
//

#ifndef SOLAR_SYSTEM_APP_H
#define SOLAR_SYSTEM_APP_H

#include <memory>
#include <vector>

class WindowManager;
class InputManager;
class Camera;
class SceneRenderer;
class UIRenderer;
class SceneData;
class Engine;
class BufferManager;
class Skybox;
class Ring;
class TextRenderer;
class CelestialBodyInfoPanel;
class MeshGenerator;
class TextureManager;
class CelestialBody;

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
  // -- Engine Context Systems --
  std::unique_ptr<WindowManager> windowManager_;
  std::unique_ptr<InputManager> inputManager_;
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<SceneRenderer> sceneRenderer_;
  std::unique_ptr<UIRenderer> uiRenderer_;
  std::unique_ptr<SceneData> sceneData_;

  // -- Core Systems --
  std::unique_ptr<Engine> engine_;
  std::unique_ptr<BufferManager> bufferManager_;
  std::unique_ptr<Skybox> skybox_;
  std::unique_ptr<Ring> saturnRing_;
  std::unique_ptr<TextRenderer> textRenderer_;
  std::unique_ptr<CelestialBodyInfoPanel> bodyInfoPanel_;
  std::unique_ptr<MeshGenerator> meshGenerator_;
  std::unique_ptr<TextureManager> textureManager_;

  std::vector<std::unique_ptr<CelestialBody>> celestialBodies_;
  // int selectedPlanetIndex_;

  // Cleanup
  bool isCleanedUp = false;

  // Initialization
  bool initializeCoreSystems(BufferManager& bufferManager,
                             TextureManager& textureManager);
  bool initializePlanets(BufferManager& bufferManager,
                         TextureManager& textureManager,
                         MeshGenerator& meshGenerator);
  bool initializeRenderers(
      const std::vector<std::unique_ptr<CelestialBody>>& celestialBodies,
      TextRenderer& textRenderer, CelestialBodyInfoPanel& celestialBodyInfo);
  void setupInputConfig();

  // Helpers
  void handlePlanetSelection(const Camera& camera);
};

#endif  // SOLAR_SYSTEM_APP_H