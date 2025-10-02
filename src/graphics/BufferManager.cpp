//
// Created by Daniel on 02-Oct-25.
//

#include "BufferManager.h"
#include <iostream>

BufferManager::~BufferManager() {
  if (!bufferRegistry.empty()) {
    std::cout << "BufferManager cleaning up " << bufferRegistry.size()
              << " remaining buffer sets:\n";

    for (const auto& [vao, info] : bufferRegistry) {
      std::cout << "  - " << info.ownerName << "\n";
      glDeleteVertexArrays(1, &info.vao);
      glDeleteBuffers(1, &info.vbo);
      glDeleteBuffers(1, &info.ebo);
    }
  }

  bufferRegistry.clear();
}

BufferHandle BufferManager::createBufferSet(
    const std::string& ownerName,
    const std::vector<float>& vertexData,
    const std::vector<unsigned int>& indexData,
    const std::vector<VertexAttribute>& attributes,
    GLenum usage
) {
    unsigned int vao, vbo, ebo;
    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glGenBuffers(1, &vbo));
    GL_CHECK(glGenBuffers(1, &ebo));

    GL_CHECK(glBindVertexArray(vao));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(float),
                 vertexData.data(),
                 usage));

    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indexData.size() * sizeof(unsigned int),
                 indexData.data(),
                 usage));

    for (const auto& attr : attributes) {
        GL_CHECK(glVertexAttribPointer(attr.index, attr.size, attr.type,
                             attr.normalized, attr.stride, attr.offset));
        GL_CHECK(glEnableVertexAttribArray(attr.index));
    }

    checkVAOBinding();

    checkBufferBinding(GL_ARRAY_BUFFER, "VBO");
    checkBufferBinding(GL_ELEMENT_ARRAY_BUFFER, "EBO");

    GL_CHECK(glBindVertexArray(0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    registerBuffer(vao, vbo, ebo, ownerName,
                  vertexData.size() * sizeof(float),
                  indexData.size() * sizeof(unsigned int));

    std::cout << "Created buffer set for: " << ownerName
              << " (VAO=" << vao << ")\n";

    return BufferHandle(vao, vbo, ebo, this);
}

void BufferManager::releaseBufferSet(unsigned int vao, unsigned int vbo,
                                     unsigned int ebo) {
  auto it = bufferRegistry.find(vao);
  if (it != bufferRegistry.end()) {
    std::cout << "Releasing buffer set: " << it->second.ownerName
              << " (VAO=" << vao << ")\n";
    bufferRegistry.erase(it);
  }

  GL_CHECK(glDeleteVertexArrays(1, &vao));
  GL_CHECK(glDeleteBuffers(1, &vbo));
  GL_CHECK(glDeleteBuffers(1, &ebo));
}

void BufferManager::registerBuffer(unsigned int vao, unsigned int vbo,
                                   unsigned int ebo, const std::string& ownerName,
                                   size_t vertexSize, size_t indexSize) {
    BufferInfo info;
    info.vao = vao;
    info.vbo = vbo;
    info.ebo = ebo;
    info.ownerName = ownerName;
    info.vertexDataSize = vertexSize;
    info.indexDataSize = indexSize;
    info.active = true;

    bufferRegistry[vao] = info;
}

void BufferManager::printActiveBuffers() const {
    std::cout << "\n=== Active Buffer Sets ===\n";
    for (const auto& [vao, info] : bufferRegistry) {
        std::cout << "  " << info.ownerName
                  << " - VAO: " << info.vao
                  << ", VBO: " << info.vbo
                  << ", EBO: " << info.ebo
                  << " | VRAM: " << (info.vertexDataSize + info.indexDataSize) / 1024
                  << " KB\n";
    }
    std::cout << "Total buffer sets: " << bufferRegistry.size() << "\n";
    std::cout << "Total VRAM: " << getTotalVRAMUsage() / 1024 / 1024 << " MB\n\n";
}

size_t BufferManager::getTotalVRAMUsage() const {
    size_t total = 0;
    for (const auto& [vao, info] : bufferRegistry) {
        total += info.vertexDataSize + info.indexDataSize;
    }
    return total;
}

void BufferManager::bindVAO(unsigned int vao) {
    GL_CHECK(glBindVertexArray(vao));
}

void BufferManager::bindBuffer(GLenum target, unsigned int buffer) {
    GL_CHECK(glBindBuffer(target, buffer));
}

void BufferManager::setBufferData(GLenum target, GLsizeiptr size,
                                 const void* data, GLenum usage) {
    GL_CHECK(glBufferData(target, size, data, usage));
}