#include "planet.h"

void Planet::create(const char* texturePath)
{
    cout << "Creating planet: " << name << endl;

    meshData = engine->generateSphereMesh(1.0f, 36, 18);
    cout << "Generated mesh with " << meshData.vertices.size() << " vertices and "
         << meshData.indices.size() << " indices" << endl;

    engine->generateVAO(&VAO);
    GL_CHECK(engine->bindVAO(VAO));

    engine->generateBuffer(&VBO);
    GL_CHECK(engine->bindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CHECK(engine->setBufferData(GL_ARRAY_BUFFER,
        meshData.vertices.size() * sizeof(float),
        meshData.vertices.data(),
        GL_STATIC_DRAW));

    engine->generateBuffer(&EBO);
    GL_CHECK(engine->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GL_CHECK(engine->setBufferData(GL_ELEMENT_ARRAY_BUFFER,
        meshData.indices.size() * sizeof(unsigned int),
        meshData.indices.data(),
        GL_STATIC_DRAW));

    GL_CHECK(engine->defineVertexLayout(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GL_CHECK(engine->defineVertexLayout(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));

    cout << "OpenGL buffers created successfully for planet " << name << endl;

    checkVAOBinding();
    checkBufferBinding(GL_ARRAY_BUFFER, "VBO");
    checkBufferBinding(GL_ELEMENT_ARRAY_BUFFER, "EBO");

    texture = make_unique<Texture>();
    this->textureID = texture->generateTexture(1);
    GL_CHECK(texture->setTextureWrappingParamsInt(GL_REPEAT));
    GL_CHECK(texture->setTextureFilteringParamsInt(GL_LINEAR));

    int width = 0, height = 0, numberOfChannels = 0;
    unsigned char* data = texture->loadTextureImage(texturePath, width, height, numberOfChannels);

    if (data && width > 0 && height > 0) {
        cout << "Texture loaded: " << width << "x" << height << " (" << numberOfChannels << " channels)" << endl;

        GLenum format = GL_RGB;
        if (numberOfChannels == 1) format = GL_RED;
        else if (numberOfChannels == 3) format = GL_RGB;
        else if (numberOfChannels == 4) format = GL_RGBA;

        GL_CHECK(texture->specifyTextureImage2D(data, format, width, height, true));
        texture->freeImageData(data);
    } else {
        cout << "Failed to load texture: " << texturePath << " - using fallback color" << endl;

        unsigned char colorData[3];
        if (name == "Sun") {
            colorData[0] = 255; colorData[1] = 255; colorData[2] = 0; // Yellow
        } else if (name == "Mercury") {
            colorData[0] = 169; colorData[1] = 169; colorData[2] = 169; // Gray
        } else if (name == "Venus") {
            colorData[0] = 255; colorData[1] = 165; colorData[2] = 0; // Orange
        } else if (name == "Earth") {
            colorData[0] = 0; colorData[1] = 100; colorData[2] = 255; // Blue
        } else {
            colorData[0] = 255; colorData[1] = 255; colorData[2] = 255; // White
        }

        GL_CHECK(texture->specifyTextureImage2D(colorData, GL_RGB, 1, 1, false));
        if (data) texture->freeImageData(data);
    }

    checkTextureBinding(GL_TEXTURE0);

    try {
        shader = make_unique<Shader>("../shaders/object.vert", "../shaders/object.frag");
        GL_CHECK(shader->use());
        GL_CHECK(shader->setInt("texture", 0));
        cout << "Shader compiled successfully for planet " << name << endl;
    } catch (const std::exception& e) {
        cout << "ERROR: Failed to create shader for planet " << name << ": " << e.what() << endl;
        return;
    }

    this->created = true;
    cout << "Planet " << name << " created successfully!" << endl;

    GL_CHECK(glBindVertexArray(0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
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