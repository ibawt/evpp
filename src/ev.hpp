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

enum class Result {
  Error,
  Ok
};

}
