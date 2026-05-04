#include "Window.h"
#include "GLFW/glfw3.h"

Window::Window(unsigned int width, unsigned int height,
               const std::string &title)
    : width(width), height(height), title(title) {}

void Window::initWindow() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to init GLFW");
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
}

void Window::pollEvents() { glfwPollEvents(); }

void Window::swapBuffers() { glfwSwapBuffers(window); }

bool Window::shouldClose() { return glfwWindowShouldClose(window); }
