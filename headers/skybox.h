#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "engine.h"
#include "shader.h"
#include "texture.h"

class Skybox {
 public:
    Skybox(const std::string& textureFilename);
    // ~Skybox();

    bool initialize(Engine* engine);

    void render(const glm::mat4& view, const glm::mat4& projection);

    unsigned int getTextureID() const { return m_textureID; }

    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }

 private:
    bool loadTexture();

    std::string m_textureFilename;
    //unsigned int m_VAO, m_VBO, m_EBO;
    unsigned int m_textureID;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Texture> m_textureLoader;

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    unsigned int m_indexCount;

    bool m_initialized;
    bool m_enabled;

    static constexpr float SKYBOX_RADIUS = 1000.0f;
    static constexpr unsigned int SKYBOX_SECTORS = 36;
    static constexpr unsigned int SKYBOX_STACKS = 18;
};

#endif