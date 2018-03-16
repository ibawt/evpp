#include "texture.hpp"

namespace ev {

typedef std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> SDLSurface;

static SDLSurface new_surface(SDL_Surface* p)
{
  return SDLSurface{p, SDL_FreeSurface};
}

static SDLSurface load_surface(const std::string& file)
{
  auto p = IMG_Load(file.c_str());
  if(!p) {
    throw std::runtime_error("error loading image");
  }
  return new_surface(p);
}

static SDLSurface convert_to(const SDLSurface src, int fmt)
{
  return new_surface(SDL_ConvertSurfaceFormat(src.get(), fmt, 0));
}

Texture::Texture(const std::string &filename) {
  auto s = load_surface(filename);

  if(s->format->format != SDL_PIXELFORMAT_ABGR8888) {
    s = convert_to(std::move(s), SDL_PIXELFORMAT_ABGR8888);
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
