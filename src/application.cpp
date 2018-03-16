#include "application.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#ifndef NO_VULKAN
#include "SDL_syswm.h"
#endif

#include "SDL_events.h"

namespace ev {
#ifndef NO_VULKAN
static vk::SurfaceKHR createVulkanSurface(const vk::Instance &instance,
                                          SDL_Window *window);
static std::vector<const char *> getAvailableWSIExtensions();
#endif

static bool initGlew = false;

static void initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    throw std::runtime_error("SDL failed to init");
  }

  if (IMG_Init(IMG_INIT_PNG) < 0) {
    throw std::runtime_error("SDL_image failed to init");
  }
}

Application::Application(int32_t width, int32_t height, std::string name)
    : width(width), height(height), shouldClose(false), name(name),
      context(nullptr), window(nullptr) {
  initSDL();

#ifndef NO_VULKAN
  auto extensions = getAvailableWSIExtensions();
  std::vector<const char *> layers;
  auto appInfo = vk::ApplicationInfo()
                     .setPApplicationName(name.c_str())
                     .setApplicationVersion(1)
                     .setPEngineName("ev++")
                     .setEngineVersion(1)
                     .setApiVersion(VK_API_VERSION_1_0);

  auto instInfo =
      vk::InstanceCreateInfo()
          .setFlags(vk::InstanceCreateFlags())
          .setPApplicationInfo(&appInfo)
          .setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()))
          .setPpEnabledLayerNames(layers.data());

  vkInstance = vk::createInstance(instInfo);
#endif
  window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, width, height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

#ifndef NO_VULKAN
  vkSurface = createVulkanSurface(vkInstance, window);
#else
  context = SDL_GL_CreateContext(window);
#endif

  if (!initGlew) {
    glewInit();
    initGlew = true;
  }
}

Application::~Application() {
  close();
  SDL_Quit();
}

void Application::close() {
  shouldClose = true;
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
#ifndef NO_VULKAN
  if (vkSurface) {
    vkInstance.destroySurfaceKHR(vkSurface);
  }

  if (vkInstance) {
    vkInstance.destroy();
  }
#else
  if (context) {
    SDL_GL_DeleteContext(context);
    context = nullptr;
  }
#endif
}

void Application::show()
{
}

void Application::run() {
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  while (!shouldClose) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_KEYDOWN:
        break;
      case SDL_KEYUP:
        break;
      case SDL_QUIT:
        shouldClose = true;
        break;
      }
    }


    auto t2 = high_resolution_clock::now();
    auto dt = duration_cast<milliseconds>(t2 - t1).count();

    t1 = t2;

    update(static_cast<float>(dt * 0.0001f));
    render();

    SDL_GL_SwapWindow(window);
  }
}

#ifndef NO_VULKAN
vk::SurfaceKHR createVulkanSurface(const vk::Instance &instance,
                                   SDL_Window *window) {
  SDL_SysWMinfo windowInfo;
  SDL_VERSION(&windowInfo.version);
  if (!SDL_GetWindowWMInfo(window, &windowInfo)) {
    throw std::system_error(std::error_code(),
                            "SDK window manager info is not available.");
  }

  switch (windowInfo.subsystem) {

#if defined(SDL_VIDEO_DRIVER_ANDROID) && defined(VK_USE_PLATFORM_ANDROID_KHR)
  case SDL_SYSWM_ANDROID: {
    vk::AndroidSurfaceCreateInfoKHR surfaceInfo =
        vk::AndroidSurfaceCreateInfoKHR().setWindow(
            windowInfo.info.android.window);
    return instance.createAndroidSurfaceKHR(surfaceInfo);
  }
#endif

#if defined(SDL_VIDEO_DRIVER_MIR) && defined(VK_USE_PLATFORM_MIR_KHR)
  case SDL_SYSWM_MIR: {
    vk::MirSurfaceCreateInfoKHR surfaceInfo =
        vk::MirSurfaceCreateInfoKHR()
            .setConnection(windowInfo.info.mir.connection)
            .setMirSurface(windowInfo.info.mir.surface);
    return instance.createMirSurfaceKHR(surfaceInfo);
  }
#endif

#if defined(SDL_VIDEO_DRIVER_WAYLAND) && defined(VK_USE_PLATFORM_WAYLAND_KHR)
  case SDL_SYSWM_WAYLAND: {
    vk::WaylandSurfaceCreateInfoKHR surfaceInfo =
        vk::WaylandSurfaceCreateInfoKHR()
            .setDisplay(windowInfo.info.wl.display)
            .setSurface(windowInfo.info.wl.surface);
    return instance.createWaylandSurfaceKHR(surfaceInfo);
  }
#endif

#if defined(SDL_VIDEO_DRIVER_WINDOWS) && defined(VK_USE_PLATFORM_WIN32_KHR)
  case SDL_SYSWM_WINDOWS: {
    vk::Win32SurfaceCreateInfoKHR surfaceInfo =
        vk::Win32SurfaceCreateInfoKHR()
            .setHinstance(GetModuleHandle(NULL))
            .setHwnd(windowInfo.info.win.window);
    return instance.createWin32SurfaceKHR(surfaceInfo);
  }
#endif

#if defined(SDL_VIDEO_DRIVER_X11) && defined(VK_USE_PLATFORM_XLIB_KHR)
  case SDL_SYSWM_X11: {
    vk::XlibSurfaceCreateInfoKHR surfaceInfo =
        vk::XlibSurfaceCreateInfoKHR()
            .setDpy(windowInfo.info.x11.display)
            .setWindow(windowInfo.info.x11.window);
    return instance.createXlibSurfaceKHR(surfaceInfo);
  }
#endif

  default:
    throw std::system_error(std::error_code(),
                            "Unsupported window manager is in use.");
  }
}

std::vector<const char *> getAvailableWSIExtensions() {
  std::vector<const char *> extensions;
  extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
  extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MIR_KHR)
  extensions.push_back(VK_KHR_MIR_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
  extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WIN32_KHR)
  extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
  extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif

  return extensions;
}
#endif
}
