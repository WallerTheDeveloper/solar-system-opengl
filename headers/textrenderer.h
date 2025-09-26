//
// Created by Daniel on 26-Sep-25.
//

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "engine.h"

using namespace std;

#include "shader.h"

struct Character {
  unsigned int TextureID;  // ID handle of the glyph texture
  glm::ivec2 Size;         // Size of glyph
  glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
  unsigned int Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer {
 public:
  TextRenderer(Engine* engine);
  ~TextRenderer();

  bool initialize();
  void renderText(const std::string& text, float x, float y, float scale,
                  glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
  void cleanup();
  void setScreenSize();

 private:
  Engine* engine;
  std::map<char, Character> Characters;
  std::unique_ptr<Shader> textShader;
  unsigned int VAO = 0, VBO = 0;

  bool loadFont();
};

#endif  // TEXTRENDERER_H