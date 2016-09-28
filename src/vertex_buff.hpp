#pragma once

#include "ev.hpp"
#include <vector>
#include <memory>

namespace ev {

template<class T>
class VertexBuff : Object
{
 public:
  static const auto DEFAULT_COUNT = 64;
  explicit VertexBuff(size_t n) : id(0), buff(n) {
    set_capacity(n);
  }
  VertexBuff() : VertexBuff(DEFAULT_COUNT) {}
  virtual ~VertexBuff() {
    dispose();
  }

  void set_capacity(size_t newsize)
  {
    if(!newsize) {
      throw std::runtime_error("invalid size");
    }

    dispose();

    glGenBuffers(1, &id);
    if(!id) {
      throw std::runtime_error("glGenBuffers failed");
    }

    bind();

    buff.reserve(newsize);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T)*newsize, buff.data(), GL_DYNAMIC_DRAW);
  }

  void bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, id);
  }

  T* map()
  {
    bind();
    return static_cast<T*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  }

  void unmap()
  {
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }
 private:
  VertexBuff(const VertexBuff&) = delete;
  VertexBuff& operator=(const VertexBuff&) = delete;

  GLuint id;
  std::vector<T> buff;

  void dispose()
  {
    if( id ) {
      glDeleteBuffers(1, &id);
      id = 0;
    }
  }
};
}
