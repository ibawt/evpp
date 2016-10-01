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
  virtual ~Application();


  Result show();
  Result close();
  Result run();

  int32_t get_height() const { return height; }
  int32_t get_width() const { return width; }

  virtual void render() = 0;
  virtual void update(const float dt) = 0;
private:
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;

  #ifndef NO_VULKAN
  vk::Instance vkInstance = VK_NULL_HANDLE;
  vk::SurfaceKHR vkSurface = VK_NULL_HANDLE;
  #endif
  int32_t        width = 0;
  int32_t        height = 0;
  bool           shouldClose = false;
  std::string    name;
  SDL_GLContext  context = nullptr;
  SDL_Window    *window = nullptr;
};

}
