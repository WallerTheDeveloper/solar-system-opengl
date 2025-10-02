//
// Created by Daniel on 02-Oct-25.
//

#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <glad/glad.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "../utils/debug_utils.h"
#include "BufferHandle.h"

struct VertexAttribute {
  unsigned int index;
  int size;
  GLenum type;
  GLboolean normalized;
  int stride;
  const void* offset;
};

struct BufferInfo {
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
  std::string ownerName;
  size_t vertexDataSize;
  size_t indexDataSize;
  bool active;
};

class BufferManager {
 public:
  BufferManager() = default;
  ~BufferManager();

  BufferHandle createBufferSet(const std::string& ownerName,
                               const std::vector<float>& vertexData,
                               const std::vector<unsigned int>& indexData,
                               const std::vector<VertexAttribute>& attributes,
                               GLenum usage = GL_STATIC_DRAW);

  unsigned int createVAO();
  unsigned int createBuffer();
  void setupVertexAttributes(unsigned int vao,
                             const std::vector<VertexAttribute>& attributes);

  void bindVAO(unsigned int vao);
  void bindBuffer(GLenum target, unsigned int buffer);
  void setBufferData(GLenum target, GLsizeiptr size, const void* data,
                     GLenum usage);

  void releaseBufferSet(unsigned int vao, unsigned int vbo, unsigned int ebo);

  // Diagnostics
  void printActiveBuffers() const;
  size_t getActiveBufferCount() const { return bufferRegistry.size(); }
  size_t getTotalVRAMUsage() const;

 private:
  std::unordered_map<unsigned int, BufferInfo> bufferRegistry;

  void registerBuffer(unsigned int vao, unsigned int vbo, unsigned int ebo,
                      const std::string& ownerName, size_t vertexSize,
                      size_t indexSize);
};

#endif