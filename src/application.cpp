#include "application.hpp"

namespace ev
{

static void initSDL()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
    throw std::runtime_error("SDL failed to init");
  }

  if(IMG_Init(IMG_INIT_PNG) < 0) {
    throw std::runtime_error("SDL_image failed to init");
  }
}

static void initGL()
{
  GLenum err = glewInit();
  if(err != GLEW_OK) {
    throw std::runtime_error("GLEW failed to init");
  }
}

Application::Application(int32_t width, int32_t height, std::string name) : width(width), height(height), name(name), context(nullptr), window(nullptr)
{
  initSDL();
  initGL();
}

Application::~Application()
{
  Close();
}

Result Application::Close()
{
  shouldClose = true;
  if(context) {
    SDL_GL_DeleteContext(context);
    context = nullptr;
  }
  if(window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }

  return EV_OK;
}

Result Application::Show()
{
  window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if(!window) {
    return EV_FAIL;
  }

  context = SDL_GL_CreateContext(window);
  if(!context) {
    return EV_FAIL;
  }
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glViewport(0, 0, width, height);

  SDL_GL_SetSwapInterval(1);

  return EV_OK;
}

Result Application::Run()
{
  auto t1 = SDL_GetPerformanceCounter();

  while(!shouldClose) {
    SDL_Event e;

    while( SDL_PollEvent(&e)) {
      switch(e.type) {
        case SDL_KEYDOWN:
          printf("keydown\n");
          break;
        case SDL_KEYUP:
          printf("keyup\n");
          break;
        case SDL_QUIT:
          return EV_OK;
      }

      auto t2 = SDL_GetPerformanceCounter() - t1;

      auto dt = ((t2 - t2) * 1000) / SDL_GetPerformanceFrequency();

      Update(dt);
      Render();

      SDL_GL_SwapWindow(window);
    }
  }
  return EV_OK;
}

}
