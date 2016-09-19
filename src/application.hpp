#pragma once
#include "ev.hpp"
#include <string>

namespace ev {

class Application : public Object
{
 public:
  Application(int32_t width, int32_t height, std::string name);
  ~Application();

  Result Show();
  Result Close();
  Result Run();

  virtual void Render() = 0;
  virtual void Update(const float dt) = 0;
 private:
  int32_t width;
  int32_t height;
  std::string name;
  bool           shouldClose;
  SDL_GLContext  context;
  SDL_Window    *window;
};

}
