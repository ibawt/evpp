#pragma once
#include "ev.hpp"

#ifndef NO_VULKAN
#include "vulkan/vulkan.hpp"
#endif

#include <string>

namespace ev {

class Application
{
public:
  Application(int width, int height, std::string name);
  virtual ~Application();


  void show();
  void close();
  void run();

  int get_height() const { return height; }
  int get_width() const { return width; }

  virtual void render() = 0;
  virtual void update(const float dt) = 0;
private:
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;

  #ifndef NO_VULKAN
  vk::Instance vkInstance = VK_NULL_HANDLE;
  vk::SurfaceKHR vkSurface = VK_NULL_HANDLE;
  #endif
  int            width = 0;
  int            height = 0;
  bool           shouldClose = false;
  std::string    name = "ev application";
  SDL_GLContext  context = nullptr;
  SDL_Window    *window = nullptr;
};

}
