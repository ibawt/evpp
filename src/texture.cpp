#include "texture.hpp"

namespace ev {
Texture::Texture(const std::string &filename) : id(0) {
  SDL_Surface *s = IMG_Load(filename.c_str());
  if (!s) {
    throw std::runtime_error("texture not found!: " + filename);
  }

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, s->pixels);

  size.width = static_cast<float>(s->w);
  size.height = static_cast<float>(s->h);

  SDL_FreeSurface(s);
}

Texture::~Texture() {
  if (id) {
    glDeleteTextures(1, &id);
    id = 0;
  }
}

}
