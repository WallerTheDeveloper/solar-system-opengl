//
// Created by Daniel on 04-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_MESHGENERATOR_H
#define SOLAR_SYSTEM_OPENGL_MESHGENERATOR_H
#include <vector>
#include "../../utils/math_utils.h"

struct ObjectMeshData {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  unsigned int indicesCount;
};

struct SphereMeshData : ObjectMeshData {};
struct BoxMeshData : ObjectMeshData {};

class MeshGenerator {
public:
  SphereMeshData generateSphereMesh(float radius, unsigned int sectorCount,
                                    unsigned int stackCount);
};
#endif  // SOLAR_SYSTEM_OPENGL_MESHGENERATOR_H
