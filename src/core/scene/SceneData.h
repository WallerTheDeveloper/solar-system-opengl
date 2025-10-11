//
// Created by Daniel on 05-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_SCENEDATA_H
#define SOLAR_SYSTEM_OPENGL_SCENEDATA_H

#include <memory>
#include <vector>

class CelestialBody;

class SceneData {
public:
  std::vector<std::unique_ptr<CelestialBody>> celestialBodies_;
};

#endif  // SOLAR_SYSTEM_OPENGL_SCENEDATA_H
