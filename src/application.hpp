#pragma once
#include "ev.hpp"
#include "glm.hpp"
#include "SDL_syswm.h"
#include "vulkan\vulkan.hpp"
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
	vk::Instance vkInstance;
	vk::SurfaceKHR vkSurface;
	int32_t width;
	int32_t height;
	std::string name;
	bool           shouldClose;
	SDL_GLContext  context;
	SDL_Window    *window;
};

}
