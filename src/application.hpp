#pragma once
#include "ev.hpp"

#ifndef NO_VULKAN
#include "vulkan/vulkan.hpp"
#endif

#include <string>

namespace ev {

class Application : public Object
{
public:
  Application(int32_t width, int32_t height, std::string name);
  ~Application();

  Result show();
  Result close();
  Result run();

  virtual void render() = 0;
  virtual void update(const float dt) = 0;
private:
  #ifndef NO_VULKAN
  vk::Instance vkInstance;
  vk::SurfaceKHR vkSurface;
  #endif
  int32_t width;
  int32_t height;
  std::string name;
  bool           shouldClose;
  SDL_GLContext  context;
  SDL_Window    *window;
};

}
