#pragma once
#include "ev.hpp"
#include <string>

namespace ev {
class Texture
{
 public:
  Texture(const std::string& filename);
  Texture(Texture&& other) : id(other.id) { other.id = 0; }
  Texture& operator=(Texture&& other) { id = other.id; other.id = 0; return *this; }
  virtual ~Texture();

  void bind() const { glBindTexture(GL_TEXTURE_2D, id); }
  const Size& get_size() { return size; }
  const Size& get_size() const { return size; }
 private:
  Texture() = delete;
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  GLuint id = 0;
  Size size = { 0, 0 };
};
}
