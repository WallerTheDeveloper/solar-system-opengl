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
      currentAngle(bodyProperties.currentRotationAngle),
      position(bodyProperties.position),
      velocity(bodyProperties.velocity),
      bufferManager_(bufferManager),
      meshGenerator_(meshGenerator),
      textureManager_(textureManager),
      props_(bodyProperties),
      hasRing(bodyProperties.hasRing) {
  std::cout << "Creating planet: " << type << std::endl;

  if (hasRing) {
    createRing();
  }
  meshData = meshGenerator_.generateSphereMesh(1.0f, 36, 18);
  std::cout << "Generated mesh with " << meshData.vertices.size()
            << " vertices and " << meshData.indices.size() << " indices"
            << std::endl;

  std::vector<VertexAttribute> attributes = {
      // position
      {0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0},
      // texCoord
      {1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
       (void*)(3 * sizeof(float))}};
  bufferHandle_ = bufferManager_.createBufferSet("Body_" + typeToString(type),
                                                 meshData.vertices,
                                                 meshData.indices, attributes);

  std::cout << "OpenGL buffers created successfully for planet " << type
            << std::endl;

  this->textureID = textureManager_.createTexture(bodyProperties.texturePath, GL_TEXTURE_2D,
                                                  GL_REPEAT, GL_LINEAR);

  try {
    shader = std::make_unique<Shader>("../shaders/object.vert",
                                      "../shaders/object.frag");
    GL_CHECK(shader->use());
    GL_CHECK(shader->setInt("texture", 0));
    std::cout << "Shader compiled successfully for planet " << type
              << std::endl;
  } catch (const std::exception& e) {
    std::cout << "ERROR: Failed to create shader for planet " << type << ": "
              << e.what() << std::endl;
    return;
  }

  this->created = true;
  std::cout << "Planet " << type << " created successfully!" << std::endl;
}

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

    const float ORBIT_RADIUS = this->semiMajorAxis;

    this->position.x = ORBIT_RADIUS * cos(this->currentAngle);
    this->position.z = ORBIT_RADIUS * sin(this->currentAngle);
    this->position.y = 0.0f;

    float speed = angularVelocity * ORBIT_RADIUS;
    this->velocity.x = -speed * sin(this->currentAngle);
    this->velocity.z = speed * cos(this->currentAngle);
    this->velocity.y = 0.0f;

    // this->props_.position = this->position;
    // this->props_.velocity = this->velocity;
    // this->props_.currentRotationAngle = this->currentAngle;
  }
}

void CelestialBody::createRing() {
  try {
    std::vector ringVertices = {
      // Positions (x, y, z)    // Texture coords (u, v)
      -3.0f, 0.0f, -3.0f,       0.0f, 0.0f,  // Bottom left
      3.0f,  0.0f, -3.0f,       1.0f, 0.0f,  // Bottom right
      3.0f,  0.0f, 3.0f,        1.0f, 1.0f,  // Top right
      -3.0f, 0.0f, 3.0f,        0.0f, 1.0f   // Top left
  };

    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

    std::vector<VertexAttribute> attributes = {
      // position
      {0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0},
      // texCoord
      {1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
       (void*)(3 * sizeof(float))}};

    ringBufferHandle_ = bufferManager_.createBufferSet("Saturn_Ring", ringVertices,
                                                  indices, attributes);

    ringTextureID = textureManager_.createTexture("../textures/saturn_ring.png",
                                              GL_TEXTURE_2D,
                                              GL_CLAMP_TO_EDGE,
                                              GL_LINEAR);

    ringShader = std::make_unique<Shader>("../shaders/ring.vert",
                                      "../shaders/ring.frag");
  } catch (std::exception& e) {
    std::cout << "Failed to create ring: " << e.what() << std::endl;
  }
}

void CelestialBody::renderRing(const glm::mat4& model, const glm::mat4& view,
                  const glm::mat4& projection) const {
  bool cullFaceWasEnabled = glIsEnabled(GL_CULL_FACE);
  bool depthTestWasEnabled = glIsEnabled(GL_DEPTH_TEST);

  // Disable face culling (ring should be visible from both sides)
  if (cullFaceWasEnabled) {
    glDisable(GL_CULL_FACE);
  }

  // CRITICAL: Keep depth testing ON but disable depth writing
  // This prevents the ring from blocking objects behind it
  if (!depthTestWasEnabled) {
    glEnable(GL_DEPTH_TEST);
  }
  glDepthMask(GL_FALSE);  // Don't write to depth buffer

  // Enable blending for transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  ringShader->use();
  ringShader->setMat4("model", model);
  ringShader->setMat4("view", view);
  ringShader->setMat4("projection", projection);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ringTextureID);
  ringShader->setInt("ringTexture", 0);

  glBindVertexArray(ringBufferHandle_.getVAO());
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glDepthMask(GL_TRUE);

  if (cullFaceWasEnabled) {
    glEnable(GL_CULL_FACE);
  }

  if (!depthTestWasEnabled) {
    glDisable(GL_DEPTH_TEST);
  }

  glDisable(GL_BLEND);
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

  glBindVertexArray(bufferHandle_.getVAO());
  glDrawElements(GL_TRIANGLES, meshData.indicesCount, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);

  if (hasRing) {
    renderRing(model, view, projection);
  }
}

void CelestialBody::update(float deltaTime) {
  updateOrbitalPositions(deltaTime);
}