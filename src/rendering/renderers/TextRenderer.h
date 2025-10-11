//
// Created by Daniel on 26-Sep-25.
//

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>

#include <graphics/buffer/BufferHandle.h>

class Shader;

struct Character {
  unsigned int TextureID;  // ID handle of the glyph texture
  glm::ivec2 Size;         // Size of glyph
  glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
  unsigned int Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer {
 public:
  TextRenderer(BufferManager& bufferManager, int screenWidth, int screenHeight);
  ~TextRenderer();

  void create();
  void renderText(const std::string& text, float x, float y, float scale,
                  glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
  void cleanup();
  void setScreenSize() const;

 private:
  BufferHandle bufferHandle_;
  BufferManager& bufferManager_;

  std::map<char, Character> Characters;
  std::unique_ptr<Shader> textShader;
  const int screenWidth_;
  const int screenHeight_;

  bool loadFont();
};

#endif  // TEXTRENDERER_H