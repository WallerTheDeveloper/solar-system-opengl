#ifndef PLANET_H
#define PLANET_H

#include <memory>

#include "celestialbody.h"
#include "shader.h"
#include "texture.h"

using namespace std;

class Planet : public CelestialBody {
 public:
  using CelestialBody::CelestialBody;
  void create(const char* texturePath) override;
  void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;

 private:
  bool created = false;
  unsigned int textureID;
  unsigned int VAO, VBO, EBO;
  SphereMeshData meshData;
  std::unique_ptr<Texture> texture;
  std::unique_ptr<Shader> shader;
};
#endif  // !PLANET_H