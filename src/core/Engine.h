#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "../utils/debug_utils.h"
#include "../utils/math_utils.h"
#include "Camera.h"
#include "Texture.h"

struct ObjectMeshData {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  unsigned int indicesCount;
};

struct SphereMeshData : ObjectMeshData {};
struct BoxMeshData : ObjectMeshData {};

class Engine {
 public:
  const unsigned int SCR_WIDTH = 1920;
  const unsigned int SCR_HEIGHT = 1080;
  Camera camera;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  static Engine& getInstance();
  static bool isEngineInitialized();

  Engine(std::string windowName, bool enable_gl_depth_test);
  ~Engine();

  // Rendering
  void render(const std::function<void()>& renderCallback = nullptr);
  void renderIndices(unsigned int VAO, unsigned int indicesCount,
                     bool unbind = false);

  // Buffers
  void generateVAO(unsigned int* VAO);
  void generateBuffer(unsigned int* buffer);

  void bindVAO(unsigned int VAO);
  void bindBuffer(GLenum target, unsigned int buffer);

  void setBufferData(GLenum target, GLsizeiptr size, const void* data,
                     GLenum usage);
  void defineVertexLayout(unsigned int shaderLayoutIndex, int size, GLenum type,
                          GLboolean normalizeData, int stride,
                          const void* offset);

  void unbindVBO();
  void unbindVAO();
  void deleteVAO(int size, unsigned int& VAO);
  void deleteBuffers(int size, const unsigned int& buffer);

  // Primitives mesh generation
  SphereMeshData generateSphereMesh(float radius, unsigned int sectorCount,
                                    unsigned int stackCount);
  BoxMeshData generateBoxMesh(float width = 2.0f, float height = 2.0f,
                              float depth = 2.0f);

  // Textures
  unsigned int addTextureToObject(std::string path, GLenum target,
                                  GLint wrapping, GLint filtering);
  void renderTexture2D(GLenum textureUnit, unsigned int textureID);
  unsigned int createCubemap(std::vector<std::string> faces);

  static std::function<void()> onLeftClickCallback;

 private:
  static Engine* instance;
  GLFWwindow* window;
  std::string windowName;
  std::unique_ptr<Texture> texture;

  bool isInitialized;
  float lastMouseX = (float)SCR_WIDTH / 2.0f;
  float lastMouseY = (float)SCR_HEIGHT / 2.0f;
  bool firstMouse = true;

  bool enableCullFaceState = false;

  // Fullscreen state
  bool isFullscreen = false;
  int windowedWidth = static_cast<int>(SCR_WIDTH);
  int windowedHeight = static_cast<int>(SCR_HEIGHT);
  int windowedPosX = 0;
  int windowedPosY = 0;

  // Core systems
  void initGLFW();
  void initGLAD();
  void processInput(GLFWwindow* window);
  void toggleFullscreen();

  // Input callbacks
  static void framebuffer_size_callback(GLFWwindow* window, int width,
                                        int height);
  static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
  static void scroll_callback(GLFWwindow* window, double xoffset,
                              double yoffset);
  static void mouse_button_callback(GLFWwindow* window, int button, int action,
                                    int mods);
  static void key_callback(GLFWwindow* window, int key, int scancode,
                           int action, int mods);

  // Window
  GLFWwindow* createWindow(std::string name, int width, int height);
};

#endif