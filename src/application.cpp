#include "application.hpp"

namespace ev
{
  Application::Application(int32_t width, int32_t height, std::string name)
    : width(width), height(height), name(name), context(nullptr), window(nullptr)
  {
    SDL_Init(SDL_INIT_EVERYTHING);
  }

  void Application::Close()
  {
    if(context) {
      SDL_GL_DeleteContext(context);
      context = nullptr;
    }
    if(window) {
      SDL_DestroyWindow(window);
      window = nullptr;
    }
  }

  void Application::Show()
  {
    if(window) {
      Close();
    }
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    context = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(1);
    IMG_Init(IMG_INIT_PNG);
  }
}
