#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <glm/glm.hpp>

#include "../core/Engine.h"
#include "../core/Shader.h"
#include "../core/Texture.h"

#ifndef SUN_MASS
#define SUN_MASS 1.989e30f
#endif

#ifndef G_CONST
#define G_CONST 6.674e-11f
#endif

class CelestialBody {
 public:
  enum BodyType {
    Sun,
    Mercury,
    Venus,
    Earth,
    Mars,
    Jupiter,
    Saturn,
    Uranus,
    Neptune
  };
  CelestialBody(Engine* engine, BodyType bodyType, float mass, float radius,
                float semiMajorAxis, float eccentricity, float orbitalPeriod,
                float currentAngle, glm::vec3 position, glm::vec3 velocity);
  ~CelestialBody() = default;

  BodyType type;
  float mass;           // kg
  float radius;         // meters (for rendering scale)
  float semiMajorAxis;  // meters (orbital radius)
  float eccentricity;   // 0 = circle, <1 = ellipse
  float orbitalPeriod;  // seconds
  float currentAngle;   // current position in orbit (radians)
  glm::vec3 position;
  glm::vec3 velocity;

  void updateOrbitalPositions(float deltaTime);
  void create(const char* texturePath);
  void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const;

 private:
  Engine* engine;
  bool created = false;
  unsigned int textureID;
  unsigned int VAO, VBO, EBO;
  SphereMeshData meshData;
  std::unique_ptr<Texture> texture;
  std::unique_ptr<Shader> shader;
};

#endif