#include "../headers/CelestialBody.h"

CelestialBody::CelestialBody(Engine* engine, BodyType bodyType, float mass,
                             float radius, float semiMajorAxis,
                             float eccentricity, float orbitalPeriod,
                             float currentAngle, glm::vec3 position,
                             glm::vec3 velocity)
    : engine(engine),
      type(bodyType),
      mass(mass),
      radius(radius),
      semiMajorAxis(semiMajorAxis),
      eccentricity(eccentricity),
      orbitalPeriod(orbitalPeriod),
      currentAngle(currentAngle),
      position(position),
      velocity(velocity) {}

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

  meshData = engine->generateSphereMesh(1.0f, 36, 18);
  std::cout << "Generated mesh with " << meshData.vertices.size()
            << " vertices and " << meshData.indices.size() << " indices"
            << std::endl;

  engine->generateVAO(&VAO);
  GL_CHECK(engine->bindVAO(VAO));

  engine->generateBuffer(&VBO);
  GL_CHECK(engine->bindBuffer(GL_ARRAY_BUFFER, VBO));
  GL_CHECK(engine->setBufferData(GL_ARRAY_BUFFER,
                                 meshData.vertices.size() * sizeof(float),
                                 meshData.vertices.data(), GL_STATIC_DRAW));

  engine->generateBuffer(&EBO);
  GL_CHECK(engine->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
  GL_CHECK(engine->setBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                 meshData.indices.size() * sizeof(unsigned int),
                                 meshData.indices.data(), GL_STATIC_DRAW));

  GL_CHECK(engine->defineVertexLayout(0, 3, GL_FLOAT, GL_FALSE,
                                      5 * sizeof(float), (void*)0));
  GL_CHECK(engine->defineVertexLayout(
      1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));

  checkVAOBinding();
  checkBufferBinding(GL_ARRAY_BUFFER, "VBO");
  checkBufferBinding(GL_ELEMENT_ARRAY_BUFFER, "EBO");

  std::cout << "OpenGL buffers created successfully for planet " << type
            << std::endl;

  this->textureID = engine->addTextureToObject(texturePath, GL_TEXTURE_2D,
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

  GL_CHECK(glBindVertexArray(0));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void CelestialBody::render(glm::mat4 model, glm::mat4 view,
                           glm::mat4 projection) const {
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

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, meshData.indicesCount, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}