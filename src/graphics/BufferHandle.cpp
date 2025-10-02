//
// Created by Daniel on 02-Oct-25.
//

#include "BufferHandle.h"

#include "BufferManager.h"

BufferHandle::BufferHandle(unsigned int vao, unsigned int vbo, unsigned int ebo,
                           BufferManager* manager)
    : vao(vao), vbo(vbo), ebo(ebo), manager(manager) {}

BufferHandle::~BufferHandle() { release(); }

BufferHandle::BufferHandle(BufferHandle&& other) noexcept
    : vao(other.vao), vbo(other.vbo), ebo(other.ebo), manager(other.manager) {
  other.vao = 0;
  other.vbo = 0;
  other.ebo = 0;
  other.manager = nullptr;
}

BufferHandle& BufferHandle::operator=(BufferHandle&& other) noexcept {
  if (this != &other) {
    release();
    vao = other.vao;
    vbo = other.vbo;
    ebo = other.ebo;
    manager = other.manager;
    other.vao = 0;
    other.vbo = 0;
    other.ebo = 0;
    other.manager = nullptr;
  }
  return *this;
}

void BufferHandle::release() {
  if (vao == 0) {
    return;
  }

  if (manager != nullptr) {
    manager->releaseBufferSet(vao, vbo, ebo);
  } else {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
  }

  vao = vbo = ebo = 0;
  manager = nullptr;
}
