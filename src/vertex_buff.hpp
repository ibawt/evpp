#pragma once

#include "ev.hpp"
#include <vector>
#include <memory>

namespace ev {

template<class T, GLenum target = GL_ARRAY_BUFFER, GLenum draw_mode = GL_DYNAMIC_DRAW>
class VertexBuff
{
 public:
  static const auto DEFAULT_COUNT = 64;
  VertexBuff(size_t n) : id(0), buff(n) {
    set_capacity(n);
  }
  VertexBuff() : VertexBuff(DEFAULT_COUNT) {}
  ~VertexBuff() {
    dispose();
  }

  VertexBuff(const VertexBuff&& other)
    : buff(std::move(other.buff)), id(other.id)
  {
    other.id = 0;
  }

  VertexBuff& operator=(const VertexBuff&& other) {
    buff = std::move(other.buff);
    id = other.id;
    other.id = 0;
    return *this;
  }

  void set_capacity(size_t newsize)
  {
    if(!newsize) {
      throw EVException("invalid size");
    }

    dispose();

    glGenBuffers(1, &id);
    if(!id) {
      throw EVException("glGenBuffers failed");
    }

    bind();

    buff.resize(newsize);
    std::memset( buff.data(), 0, sizeof(T)*buff.size());
    glBufferData(target, sizeof(T)*newsize, buff.data(), draw_mode);
  }

  void bind() const
  {
    glBindBuffer(target, id);
  }

  T* map()
  {
    bind();
    return static_cast<T*>(glMapBuffer(target, GL_WRITE_ONLY));
  }

  void unmap()
  {
    glUnmapBuffer(target);
  }

  size_t size() const { return buff.size(); }

  // Use this over map and unmap() for auto unmapping
  typedef std::unique_ptr<T[], std::function<void(T*)>> MappedBuffer;
  MappedBuffer mapped_buffer() {
    return MappedBuffer{ map(), [](T*) { glUnmapBuffer(target); }};
  }
 private:
  VertexBuff(const VertexBuff&) = delete;
  VertexBuff& operator=(const VertexBuff&) = delete;

  GLuint id = 0;
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
