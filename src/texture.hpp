#pragma once
#include "ev.hpp"
#include <string>

namespace ev {
class Texture : Object
{
 public:
  explicit Texture(const std::string& filename);
  virtual ~Texture();

  void bind() const { glBindTexture(GL_TEXTURE_2D, id); }
  const Size& get_size() { return size; }
  const Size& get_size() const { return size; }
 private:
  Texture() = delete;
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  GLuint id;
  Size size;
};
}
