#pragma once

#define NO_SDL_GLEXT
#ifdef _WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#endif

namespace ev {
class Object
{
};

struct Vec2 {
  float x;
  float y;
  Vec2(float x, float y) : x(x), y(y) {}
  Vec2() : x(0.0f), y(0.0f) {}
};

struct BatchVertex
{
  float x,y,u,v,rotation,scale,tx,ty,opacity;

  BatchVertex() : x(0.0f), y(0.0f), u(0.0f), v(0.0f),
	              rotation(0.0f), scale(0.0f), tx(0.0f),
	              ty(0.0f), opacity(0.0f) {}
};

struct Size {
  float width;
  float height;
  Size(float w, float h) : width(w), height(h) {}
  Size() : width(0.0f), height(0.0f) {}
};

struct Rectangle {
  Vec2 origin;
  Size size;

  Rectangle(float x, float y, float w, float h) :
      origin(x, y), size(w, h) {}
  Rectangle(Vec2 origin, Size size) : origin(origin), size(size) {}
  Rectangle() {}

  float left() const { return origin.x; }
  float right() const { return origin.x + size.width; }
  float top() const { return origin.y;  }
  float bottom() const { return origin.y + size.height; }
};

enum class Result {
  Error,
  Ok
};

}
