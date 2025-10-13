#include "CelestialBody.h"

#include <utils/debug_utils.h>
#include <utils/math_utils.h>

std::string CelestialBody::typeToString(BodyType body) {
    switch (body) {
      case Sun: return "Sun";
      case Mercury: return "Mercury";
      case Venus: return "Venus";
      case Earth: return "Earth";
      case Mars: return "Mars";
      case Jupiter: return "Jupiter";
      case Saturn: return "Saturn";
      case Uranus: return "Uranus";
      case Neptune: return "Neptune";
      default: return "Unknown Body Type";
  }
}

CelestialBody::CelestialBody(const BodyProps& bodyProperties,
                             BufferManager& bufferManager,
                             MeshGenerator& meshGenerator,
                             TextureManager& textureManager)
    : type(bodyProperties.type),
      mass(bodyProperties.mass),
      radius(bodyProperties.radius),
      semiMajorAxis(bodyProperties.semiMajorAxis),
      eccentricity(bodyProperties.eccentricity),
      orbitalPeriod(bodyProperties.orbitalPeriod),
      currentAngle(bodyProperties.currentAngle),
      position(bodyProperties.position),
      velocity(bodyProperties.velocity),
      bufferManager_(bufferManager),
      meshGenerator_(meshGenerator),
      textureManager_(textureManager),
      props_(bodyProperties)
{}
void CelestialBody::updateOrbitalPositions(float deltaTime) {
  if (this->type == Sun) {
    return;
  }

  if (this->orbitalPeriod > 0.0f) {
    float angularVelocity = (2.0f * M_PI) / this->orbitalPeriod;

    this->currentAngle += angularVelocity * deltaTime;

    if (this->currentAngle > 2.0f * M_PI) {
      this->currentAngle -= 2.0f * M_PI;
    }

    this->position.x = this->semiMajorAxis * cos(this->currentAngle);
    this->position.z = this->semiMajorAxis * sin(this->currentAngle);
    this->position.y = 0.0f;  // Assuming all orbits are in the same plane

    float speed = angularVelocity * this->semiMajorAxis;
    this->velocity.x = -speed * sin(this->currentAngle);
    this->velocity.z = speed * cos(this->currentAngle);
    this->velocity.y = 0.0f;
  }
}
void CelestialBody::create(const char* texturePath) {
  std::cout << "Creating planet: " << type << std::endl;

  meshData = meshGenerator_.generateSphereMesh(1.0f, 36, 18);
  std::cout << "Generated mesh with " << meshData.vertices.size()
            << " vertices and " << meshData.indices.size() << " indices"
            << std::endl;

  std::vector<VertexAttribute> attributes = {
      // position
      {0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0},
      // texCoord
      {1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))}
  };
  bufferHandle = bufferManager_.createBufferSet(
        "Body_" + typeToString(type),
        meshData.vertices,
        meshData.indices,
        attributes
    );

  std::cout << "OpenGL buffers created successfully for planet " << type
            << std::endl;

  this->textureID = textureManager_.createTexture(texturePath, GL_TEXTURE_2D,
                                               GL_REPEAT, GL_LINEAR);

  try {
    shader = std::make_unique<Shader>("../shaders/object.vert",
                                      "../shaders/object.frag");
    GL_CHECK(shader->use());
    GL_CHECK(shader->setInt("texture", 0));
    std::cout << "Shader compiled successfully for planet " << type
              << std::endl;
  } catch (const std::exception& e) {
    std::cout << "ERROR: Failed to create shader for planet " << type
              << ": " << e.what() << std::endl;
    return;
  }

  this->created = true;
  std::cout << "Planet " << type << " created successfully!" << std::endl;
}

void CelestialBody::render(const glm::mat4 model, const glm::mat4 view,
                           const glm::mat4 projection) const {
  if (!created) {
    std::cout << "Planet was not created. Nothing to render! Did you call "
                 "Planet::create before?"
              << std::endl;
    return;
  }

  shader->use();

  shader->setMat4("model", model);
  shader->setMat4("view", view);
  shader->setMat4("projection", projection);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  shader->setInt("texture", 0);

  glBindVertexArray(bufferHandle.getVAO());
  glDrawElements(GL_TRIANGLES, meshData.indicesCount, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void CelestialBody::update(float deltaTime) {
  updateOrbitalPositions(deltaTime);
}