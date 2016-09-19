#define NO_SDL_GLEXT
#include "SDL.h"

int main(int argc, char **argv)
{
  auto window = SDL_CreateWindow("ev++", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 800, 600,
                                 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  return 0;
}
