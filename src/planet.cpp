#include "../headers/planet.h"

void Planet::create(const char* texturePath) 
{
  cout << "Creating planet: " << endl;

  meshData = engine->generateSphereMesh(1.0f, 36, 18);

  engine->generateVAO(&VAO);

  engine->generateBuffer(&VBO);

  engine->bindVAO(VAO);

  engine->bindBuffer(GL_ARRAY_BUFFER, VBO);
  engine->setBufferData(GL_ARRAY_BUFFER, 
      meshData.vertices.size() * sizeof(float),
      meshData.vertices.data(),
      GL_STATIC_DRAW);

  engine->generateBuffer(&EBO);
  engine->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  engine->setBufferData(GL_ELEMENT_ARRAY_BUFFER, 
      meshData.indices.size() * sizeof(unsigned int), 
      meshData.indices.data(), 
      GL_STATIC_DRAW);

  engine->defineVertexLayout(0, 3, GL_FLOAT, GL_FALSE, 5, (void*)0);
  engine->defineVertexLayout(1, 2, GL_FLOAT, GL_FALSE, 5, (void*)(3 * sizeof(float)));

  texture = make_unique<Texture>();

  this->textureID = texture->generateTexture(1);
  texture->setTextureWrappingParamsInt(GL_REPEAT);
  texture->setTextureFilteringParamsInt(GL_LINEAR);

  cout << "Buffers created successfully for planet " << name << endl;
  int width = 0, height, numberOfChannels;

  unsigned char* data = texture->loadTextureImage(texturePath, width, height, numberOfChannels);

  if (data) {
    texture->specifyTextureImage2D(data, GL_RGB, width, height, true);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  texture->freeImageData(data);


  shader = make_unique<Shader>("shaders/object.vert", "shaders/object.frag");
  shader->setInt("texture", 0);
  shader->use();

  this->created = true;
  cout << "Planet " << " created!" << endl;
} 

void Planet::render(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    if (!created) 
    {
        cout << "Planet was not created. Nothing to render! Did you call "
                "Planet::create before?"
             << endl;
      return;
    }

    texture->setTextureActive2D(GL_TEXTURE0, textureID);

    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, meshData.indicesCount, GL_UNSIGNED_INT, 0);
}