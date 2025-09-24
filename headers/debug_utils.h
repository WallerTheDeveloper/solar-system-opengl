// debug_utils.h - Minimal version for MSVC and OpenGL 3.3
#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <glad/glad.h>

#include <iostream>
#include <string>

// Simple OpenGL Error checking macro
#define GL_CHECK(stmt)                           \
  do {                                           \
    stmt;                                        \
    checkOpenGLError(#stmt, __FILE__, __LINE__); \
  } while (0)

inline void checkOpenGLError(const char* stmt, const char* fname, int line) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::cerr << "OpenGL error " << err << " at " << fname << ":" << line
              << " - for " << stmt << std::endl;

    const char* error_string;
    switch (err) {
      case GL_INVALID_ENUM:
        error_string = "GL_INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error_string = "GL_INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error_string = "GL_INVALID_OPERATION";
        break;
      case GL_OUT_OF_MEMORY:
        error_string = "GL_OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error_string = "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
      default:
        error_string = "UNKNOWN_ERROR";
        break;
    }

    std::cerr << "Error description: " << error_string << std::endl;
    // Check current program
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    std::cout << "Current shader program: " << currentProgram << std::endl;

    // Check current VAO
    GLint currentVAO;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
    std::cout << "Current VAO: " << currentVAO << std::endl;

// Break into debugger on Windows
#ifdef _WIN32
    __debugbreak();
#else
    __builtin_trap();
#endif
  }
}

// Utility function to print OpenGL info
inline void printOpenGLInfo() {
  std::cout << "=== OpenGL Information ===" << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;

  int maxVertexAttribs;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
  std::cout << "Max Vertex Attributes: " << maxVertexAttribs << std::endl;

  int maxTextureUnits;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
  std::cout << "Max Texture Units: " << maxTextureUnits << std::endl;

  std::cout << "========================" << std::endl;
}

// Check if buffer is properly bound
inline void checkBufferBinding(GLenum target, const std::string& bufferName) {
  int boundBuffer;
  GLenum queryTarget;

  switch (target) {
    case GL_ARRAY_BUFFER:
      queryTarget = GL_ARRAY_BUFFER_BINDING;
      break;
    case GL_ELEMENT_ARRAY_BUFFER:
      queryTarget = GL_ELEMENT_ARRAY_BUFFER_BINDING;
      break;
    default:
      std::cerr << "Unknown buffer target for binding check" << std::endl;
      return;
  }

  glGetIntegerv(queryTarget, &boundBuffer);
  std::cout << bufferName << " bound to ID: " << boundBuffer << std::endl;
}

// Check VAO binding
inline void checkVAOBinding() {
  int boundVAO;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
  std::cout << "Currently bound VAO: " << boundVAO << std::endl;
}

// Check texture binding
inline void checkTextureBinding(GLenum textureUnit = GL_TEXTURE0) {
  glActiveTexture(textureUnit);
  int boundTexture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
  std::cout << "Texture unit " << (textureUnit - GL_TEXTURE0)
            << " has texture ID: " << boundTexture << " bound" << std::endl;
}

// Initialize basic debugging (without advanced debug context)
inline void initializeBasicDebugging() {
  std::cout << "Initializing basic OpenGL debugging..." << std::endl;
  printOpenGLInfo();

  // Check for common issues
  GL_CHECK(glGetError());  // Clear any existing errors

  std::cout << "Basic debugging initialized" << std::endl;
}

#endif  // DEBUG_UTILS_H