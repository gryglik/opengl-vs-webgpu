#include "Window.h"
#include "GLFW/glfw3.h"
#include <stdexcept>

Window::Window(int width, int height, const std::string &title)
    : width(width), height(height), title(title) {}

void Window::initWindow() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to init GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create window");
  }

  glfwMakeContextCurrent(window);

  gladLoadGL(glfwGetProcAddress);
}

void Window::pollEvents() { glfwPollEvents(); }

void Window::swapBuffers() { glfwSwapBuffers(window); }

bool Window::shouldClose() { return glfwWindowShouldClose(window); }
