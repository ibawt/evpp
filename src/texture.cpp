#include "texture.hpp"

class SDLImage
{
public:
  SDLImage(const std::string& s) {
    image = IMG_Load(s.c_str());
    if(!image) {
      throw std::runtime_error("image loading failure");
    }
  }
  SDLImage(SDL_Surface *s) : image(s) {
    if(!image) {
      throw std::runtime_error("image is null");
    }
  }
  ~SDLImage() {
    if(image) {
      SDL_FreeSurface(image);
    }
  }

  int format() const {
    return image->format->format;
  }

  SDLImage to_format(int pixel_format) const {
    return SDLImage(SDL_ConvertSurfaceFormat(image, pixel_format, 0));
  }

  SDL_Surface* operator->() {
    return image;
  }
  SDLImage& operator=(SDLImage &&o) {
    image = o.image;
    o.image = nullptr;
    return *this;
  }
  SDLImage(SDLImage&&o) : image(o.image) {
    o.image = nullptr;
  }
private:
  SDLImage() = delete;
  SDLImage(const SDLImage&) = delete;
  SDLImage& operator=(const SDLImage&) = delete;

  SDL_Surface *image = nullptr;
};

namespace ev {
Texture::Texture(const std::string &filename) : id(0) {
  SDLImage s(filename);

  if(s.format() != SDL_PIXELFORMAT_ABGR8888) {
    s = s.to_format(SDL_PIXELFORMAT_ABGR8888);
  }

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);

  size.width = static_cast<float>(s->w);
  size.height = static_cast<float>(s->h);
}

Texture::~Texture() {
  if (id) {
    glDeleteTextures(1, &id);
    id = 0;
  }
}
}
