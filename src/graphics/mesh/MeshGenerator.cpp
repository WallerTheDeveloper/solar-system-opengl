//
// Created by Daniel on 04-Oct-25.
//

#include "MeshGenerator.h"

#include <utils/math_utils.h>

SphereMeshData MeshGenerator::generateSphereMesh(float radius,
                                                 unsigned int sectorCount,
                                                 unsigned int stackCount) const {
  SphereMeshData data;

  float x, y, z, xy;
  float sectorStep = 2 * M_PI / sectorCount;
  float stackStep = M_PI / stackCount;
  float sectorAngle, stackAngle;

  for (unsigned int i = 0; i <= stackCount; ++i) {
    stackAngle = M_PI / 2 - i * stackStep;
    xy = radius * cosf(stackAngle);
    y = radius * sinf(stackAngle);

    for (unsigned int j = 0; j <= sectorCount; ++j) {
      sectorAngle = j * sectorStep;

      x = xy * cosf(sectorAngle);
      z = xy * sinf(sectorAngle);

      // Position
      data.vertices.push_back(x);
      data.vertices.push_back(y);
      data.vertices.push_back(z);

      // Texture coordinates
      float u = (float)j / sectorCount;
      float v = 1.0f - (float)i / stackCount;

      data.vertices.push_back(u);
      data.vertices.push_back(v);
    }
  }

  unsigned int k1, k2;
  for (unsigned int i = 0; i < stackCount; ++i) {
    k1 = i * (sectorCount + 1);
    k2 = k1 + sectorCount + 1;

    for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
      if (i != 0) {
        data.indices.push_back(k1);
        data.indices.push_back(k2);
        data.indices.push_back(k1 + 1);
      }

      if (i != (stackCount - 1)) {
        data.indices.push_back(k1 + 1);
        data.indices.push_back(k2);
        data.indices.push_back(k2 + 1);
      }
    }
  }

  data.indicesCount = data.indices.size();

  return data;
}