#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <core/Shader.h>
#include <core/texturing/TextureManager.h>

#include <graphics/buffer/BufferManager.h>
#include <graphics/buffer/BufferHandle.h>
#include <graphics/mesh/MeshGenerator.h>

#include <rendering/renderables/scene/ISceneRenderable.h>

#include <CelestialBodyTypes.h>

#include <memory>
#include <string>

#include "glm/detail/type_vec3.hpp"

class CelestialBody : public ISceneRenderable {
 public:
  std::string typeToString(BodyType body);

  const BodyProps& getBodyProps() const {
    return props_;
  }

  CelestialBody(const BodyProps& bodyProperties, BufferManager& bufferManager,
                MeshGenerator& meshGenerator, TextureManager& textureManager);
  ~CelestialBody() override = default;

  void create(const char* texturePath);

  void render(glm::mat4 model, glm::mat4 view,
              glm::mat4 projection) const override;
  void update(float deltaTime) override;

 private:
  BufferManager& bufferManager_;
  BufferHandle bufferHandle;
  MeshGenerator& meshGenerator_;
  TextureManager& textureManager_;

  bool created = false;
  unsigned int textureID;
  SphereMeshData meshData;

  std::unique_ptr<Shader> shader;

  BodyProps props_;

  BodyType type;
  float mass;
  float radius;
  float semiMajorAxis;
  float eccentricity;
  float orbitalPeriod;
  float currentAngle;
  glm::vec3 position;
  glm::vec3 velocity;

  void updateOrbitalPositions(float deltaTime);
};

#endif