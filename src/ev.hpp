#pragma once

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

namespace ev {
class Object
{
};

enum Result {
  EV_FAIL = 1,
  EV_OK = 0
};

}
