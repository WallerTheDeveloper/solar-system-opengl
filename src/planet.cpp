#include "planet.h"

void Planet::create(const char* texturePath) {
  cout << "Creating planet: " << bodyType << endl;

  meshData = engine->generateSphereMesh(1.0f, 36, 18);
  cout << "Generated mesh with " << meshData.vertices.size() << " vertices and "
       << meshData.indices.size() << " indices" << endl;

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

  cout << "OpenGL buffers created successfully for planet " << bodyType << endl;

  this->textureID = engine->addTextureToObject(texturePath, GL_REPEAT, GL_LINEAR);

  try {
    shader =
        make_unique<Shader>("../shaders/object.vert", "../shaders/object.frag");
    GL_CHECK(shader->use());
    GL_CHECK(shader->setInt("texture", 0));
    cout << "Shader compiled successfully for planet " << bodyType << endl;
  } catch (const std::exception& e) {
    cout << "ERROR: Failed to create shader for planet " << bodyType << ": "
         << e.what() << endl;
    return;
  }

  this->created = true;
  cout << "Planet " << bodyType << " created successfully!" << endl;

  GL_CHECK(glBindVertexArray(0));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
void Planet::render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
  if (!created) {
    cout << "Planet was not created. Nothing to render! Did you call "
            "Planet::create before?"
         << endl;
    return;
  }

  engine->renderTexture2D(GL_TEXTURE0, textureID);

  shader->setMat4("model", model);
  shader->setMat4("view", view);
  shader->setMat4("projection", projection);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, meshData.indicesCount, GL_UNSIGNED_INT, 0);
}