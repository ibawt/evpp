#pragma once

#define NO_SDL_GLEXT
#include "GL/glew.h"
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#endif

#include "glm.hpp"
#include "json.hpp"
#include <iostream>

namespace ev {
using mat4 = glm::mat4;
using vec2 = glm::vec2;
using json = nlohmann::json;

static inline float radian2degree(float rads) {
  return static_cast<float>(rads * 180.0f / M_PI);
}

static inline float degree2radian(float degrees) {
  return static_cast<float>(degrees * M_PI / 180.0f);
}

struct BlendMode {
  GLenum src;
  GLenum dst;

  BlendMode(GLenum s, GLenum d) : src(s), dst(d) {}
};

struct BatchVertex {
  vec2 position;
  vec2 tex;
  float rotation;
  float scale;
  vec2 translation;
  float opacity;

  BatchVertex()
      : position(0.0f, 0.0f), tex(0.0f, 0.0f), rotation(0.0f), scale(0.0f),
        translation(0.0f, 0.0f), opacity(0.0f) {}
};

class BatchVertexFiller {
public:
  virtual uint32_t fill(BatchVertex*) const = 0;
};

struct Size {
  float width;
  float height;
  Size(float w, float h) : width(w), height(h) {}
  Size() : width(0.0f), height(0.0f) {}
};

inline std::ostream &operator<<(std::ostream &os, const Size &size) {
  os << "width: " << size.width << " height: " << size.height;
  return os;
}

struct Rectangle {
  vec2 origin;
  Size size;

  Rectangle(float x, float y, float w, float h) : origin(x, y), size(w, h) {}
  Rectangle(vec2 origin, Size size) : origin(origin), size(size) {}
  Rectangle() {}

  float left() const { return origin.x; }
  float right() const { return origin.x + size.width; }
  float top() const { return origin.y; }
  float bottom() const { return origin.y + size.height; }
};

inline std::ostream &operator<<(std::ostream &os, const vec2 &v) {
  os << "x: " << v.x << ", y: " << v.y;
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const Rectangle &r) {
  os << "origin[" << r.origin << "], size[" << r.size << "]";
  return os;
}

inline vec2 to_vec2(const Size &size) { return vec2{size.width, size.height}; }
}

class EVException : public std::runtime_error {
public:
  EVException(const char *s) : std::runtime_error(s) {}
};
