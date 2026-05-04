#include "Window.h"
#include "GLFW/glfw3.h"

Window::Window(unsigned int width, unsigned int height,
               const std::string &title, Window::GraphicsAPI api)
    : width(width), height(height), title(title), api(api) {}

void Window::init() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to init GLFW");
  }

  if (api == Window::GraphicsAPI::OpenGL) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  } else if (api == Window::GraphicsAPI::WebGPU) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  }

#if defined(PLATFORM_MACOS)
  glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_COCOA);
#elif defined(PLATFORM_WINDOWS)
  glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WIN32);
#elif defined(USE_WAYLAND)
  glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
#elif defined(USE_X11)
  glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create window");
  }

  glfwMakeContextCurrent(window);

  if (api == Window::GraphicsAPI::OpenGL) {
    if (!gladLoadGL(glfwGetProcAddress)) {
      glfwTerminate();
      throw std::runtime_error("Failed to initialize glad");
    }
  }
}

void Window::pollEvents() { glfwPollEvents(); }

void Window::swapBuffers() { glfwSwapBuffers(window); }

bool Window::shouldClose() { return glfwWindowShouldClose(window); }
