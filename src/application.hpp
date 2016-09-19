#pragma once
#include "ev.hpp"
#include <string>

namespace ev
{
  class Application : public Object
  {
  public:
    Application(int32_t width, int32_t height, std::string name);
    ~Application();

    void Show();
    void Close();

    virtual void Render() = 0;
    virtual void Update(float dt) = 0;
  private:
    int32_t width;
    int32_t height;
    std::string    name;
    SDL_GLContext  context;
    SDL_Window    *window;
  };
}
