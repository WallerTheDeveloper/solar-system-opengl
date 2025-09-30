//
// Created by Daniel on 30-Sep-25.
//

#ifndef SOLAR_SYSTEM_APP_H
#define SOLAR_SYSTEM_APP_H

#include <memory>
#include <vector>

#include "CelestialBodyFactory.h"
#include "CelestialBodyPicker.h"
#include "Engine.h"
#include "PlanetInfoPanel.h"
#include "Ring.h"
#include "SceneRenderer.h"
#include "Skybox.h"
#include "TextRenderer.h"
#include "UIRenderer.h"
#include "CelestialBody.h"

// Configuration
struct AppConfig {
  static constexpr const char* WINDOW_NAME = "Solar System Simulation";
  static constexpr bool ENABLE_GL_DEPTH_TEST = true;
  static constexpr float TIME_SCALE = 1.5f;
  static const std::vector<std::string> SKYBOX_FACES;
};

class SolarSystemApp {
 public:
  SolarSystemApp();
  ~SolarSystemApp() = default;

  SolarSystemApp(const SolarSystemApp&) = delete;
  SolarSystemApp& operator=(const SolarSystemApp&) = delete;

  bool initialize();
  void run();

 private:
  // Core systems
  std::unique_ptr<Engine> engine_;
  std::unique_ptr<Skybox> skybox_;
  std::unique_ptr<Ring> saturnRing_;
  std::unique_ptr<TextRenderer> textRenderer_;
  std::unique_ptr<PlanetInfoPanel> planetInfoPanel_;

  // Rendering helpers
  std::unique_ptr<SceneRenderer> sceneRenderer_;
  std::unique_ptr<UIRenderer> uiRenderer_;

  // Scene data
  std::vector<CelestialBody> celestialBodies_;
  int selectedPlanetIndex_ = -1;

  // Time & performance tracking
  float lastFPSTime_ = 0.0f;
  float currentFPS_ = 0.0f;
  int frameCount_ = 0;

  // Initialization
  bool initializeCoreSystems();
  bool initializePlanets();
  bool initializeRenderers();
  void setupInputCallbacks();

  // Main loop
  void onFrame();
  void update(float deltaTime);
  void render(float currentTime);

  // Helpers
  void calculateFPS(float currentTime);
  void handlePlanetSelection();

  // Static callback bridge (for Engine's static callback)
  static SolarSystemApp* instance_;
  static void onLeftClickCallbackBridge();
};

#endif  // SOLAR_SYSTEM_APP_H