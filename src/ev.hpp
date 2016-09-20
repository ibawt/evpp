#pragma once

#define NO_SDL_GLEXT
#include <GL/glew.h>
#ifdef WIN32
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
  float x,y,u,v,rotation,scale,tx,y,opacity;
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

  float left() const;
  float right() const;
  float top() const;
  float bottom() const;
};

enum class Result {
  Error,
  Ok
};

}
