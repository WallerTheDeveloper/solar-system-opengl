#include "textrenderer.h"

TextRenderer::TextRenderer(Engine* engine) : engine(engine) {}

TextRenderer::~TextRenderer() {
  cleanup();
}

bool TextRenderer::initialize() {
  try {
    textShader = std::make_unique<Shader>("../shaders/uiText.vert",
                                          "../shaders/uiText.frag");
  } catch (const std::exception& e) {
    std::cerr << "ERROR: Failed to load text shader: " << e.what() << std::endl;
    return false;
  }

  glm::mat4 projection =
      glm::ortho(0.0f, static_cast<float>(engine->SCR_WIDTH), 0.0f,
                 static_cast<float>(engine->SCR_HEIGHT));
  textShader->use();
  textShader->setMat4("projection", projection);

  engine->generateVAO(&VAO);
  engine->generateBuffer(&VBO);
  engine->bindVAO(VAO);

  engine->bindBuffer(GL_ARRAY_BUFFER, VBO);
  engine->setBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

  engine->defineVertexLayout(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

  engine->unbindVBO();
  engine->unbindVAO();

  loadFont();
  std::cout << "Text renderer initialized successfully" << std::endl;
  return true;
}

bool TextRenderer::loadFont() {
  try {
    // Define simple 8x8 bitmap patterns for common characters
    // Each character is represented as 8 bytes (64 bits total)
    std::map<char, std::vector<unsigned char>> fontPatterns = {
        {' ', {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},  // Space
        {'A', {0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x00}},
        {'B', {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00}},
        {'C', {0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00}},
        {'D', {0x78, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0x78, 0x00}},
        {'E', {0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x7E, 0x00}},
        {'F', {0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00}},
        {'G', {0x3C, 0x66, 0x60, 0x6E, 0x66, 0x66, 0x3C, 0x00}},
        {'H', {0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00}},
        {'I', {0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00}},
        {'J', {0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x6C, 0x38, 0x00}},
        {'K', {0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66, 0x00}},
        {'L', {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00}},
        {'M', {0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x00}},
        {'N', {0x66, 0x76, 0x7E, 0x7E, 0x6E, 0x66, 0x66, 0x00}},
        {'O', {0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}},
        {'P', {0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x00}},
        {'Q', {0x3C, 0x66, 0x66, 0x66, 0x6E, 0x3C, 0x06, 0x00}},
        {'R', {0x7C, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x66, 0x00}},
        {'S', {0x3C, 0x66, 0x60, 0x3C, 0x06, 0x66, 0x3C, 0x00}},
        {'T', {0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}},
        {'U', {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}},
        {'V', {0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00}},
        {'W', {0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00}},
        {'X', {0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00}},
        {'Y', {0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00}},
        {'Z', {0x7E, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00}},
        {'0', {0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x3C, 0x00}},
        {'1', {0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00}},
        {'2', {0x3C, 0x66, 0x06, 0x0C, 0x30, 0x60, 0x7E, 0x00}},
        {'3', {0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0x00}},
        {'4', {0x0C, 0x1C, 0x3C, 0x6C, 0x7E, 0x0C, 0x0C, 0x00}},
        {'5', {0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00}},
        {'6', {0x3C, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00}},
        {'7', {0x7E, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00}},
        {'8', {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00}},
        {'9', {0x3C, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x78, 0x00}},
        {':', {0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00}},
        {'+', {0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00}},
        {'-', {0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00}},
        {'(', {0x0C, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00}},
        {')', {0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00}},
        {'.', {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00}},
        {',', {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30}},
    };

    // Create textures for all printable ASCII characters
    for (unsigned char c = 32; c <= 126; c++) {
      unsigned int texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);

      unsigned char charData[64];

      // Use predefined pattern if available, otherwise create default pattern
      if (fontPatterns.find(c) != fontPatterns.end()) {
        auto& pattern = fontPatterns[c];
        for (int row = 0; row < 8; row++) {
          for (int col = 0; col < 8; col++) {
            int index = row * 8 + col;
            charData[index] = (pattern[row] & (1 << (7 - col))) ? 255 : 0;
          }
        }
      } else {
        // Default pattern for undefined characters
        for (int i = 0; i < 64; i++) {
          charData[i] = ((i + c) % 3 == 0) ? 255 : 0;
        }
      }

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 8, 8, 0, GL_RED, GL_UNSIGNED_BYTE,
                   charData);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      Character character = {texture, glm::ivec2(8, 8), glm::ivec2(0, 8), 10};
      Characters.insert(std::pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
  } catch (const std::exception& e) {
    std::cerr << "ERROR: Failed to load font" << std::endl;
    return false;
  }
}

void TextRenderer::renderText(const std::string& text, float x, float y,
                              float scale, glm::vec3 color) {
  GLboolean blendEnabled = glIsEnabled(GL_BLEND);
  GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
  GLint blendSrc, blendDst;
  glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
  glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_DEPTH_TEST);

  textShader->use();
  textShader->setVec3("textColor", color);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  // Convert from top-left coordinates to OpenGL bottom-left coordinates
  float yPos =
      engine->SCR_HEIGHT - y - (8 * scale);  // Adjust for character height

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    if (Characters.find(*c) == Characters.end()) {
      continue;  // Skip characters we don't have
    }

    Character ch = Characters[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = yPos + (Characters['H'].Bearing.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f},

        {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}};

    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);

    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Advance cursor for next glyph
    x += (ch.Advance) * scale;
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Restore OpenGL state
  if (depthTestEnabled) {
    glEnable(GL_DEPTH_TEST);
  }
  if (!blendEnabled) {
    glDisable(GL_BLEND);
  } else {
    glBlendFunc(blendSrc, blendDst);
  }
}

void TextRenderer::setScreenSize() {
  if (textShader) {
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(engine->SCR_WIDTH), 0.0f,
                   static_cast<float>(engine->SCR_HEIGHT));
    textShader->use();
    textShader->setMat4("projection", projection);
  }
}

void TextRenderer::cleanup() {
  for (auto& pair : Characters) {
    glDeleteTextures(1, &pair.second.TextureID);
  }
  Characters.clear();

  if (VAO != 0) {
    glDeleteVertexArrays(1, &VAO);
    VAO = 0;
  }
  if (VBO != 0) {
    glDeleteBuffers(1, &VBO);
    VBO = 0;
  }

  std::cout << "Text renderer cleaned up" << std::endl;
}