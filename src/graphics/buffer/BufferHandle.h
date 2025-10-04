//
// Created by Daniel on 02-Oct-25.
//

#ifndef BUFFER_HANDLE_H
#define BUFFER_HANDLE_H

#include "../../utils/debug_utils.h"

class BufferHandle {
 public:
  BufferHandle() = default;
  ~BufferHandle();

  // Disable copying, enable moving
  BufferHandle(const BufferHandle&) = delete;
  BufferHandle& operator=(const BufferHandle&) = delete;
  BufferHandle(BufferHandle&& other) noexcept;
  BufferHandle& operator=(BufferHandle&& other) noexcept;

  unsigned int getVAO() const { return vao; }
  unsigned int getVBO() const { return vbo; }
  unsigned int getEBO() const { return ebo; }

  bool isValid() const { return vao != 0; }

 private:
  friend class BufferManager;

  BufferHandle(unsigned int vao, unsigned int vbo, unsigned int ebo,
               BufferManager* manager);

  unsigned int vao = 0;
  unsigned int vbo = 0;
  unsigned int ebo = 0;
  BufferManager* manager = nullptr;

  void release();
};

#endif
