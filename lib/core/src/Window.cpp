#include "Window.h"
#include <stdexcept>

WindowManager::WindowManager(int width, int height, const std::string &title)
    : width(width), height(height), title(title) {}

void WindowManager::initWindow() {
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

void WindowManager::pollEvents() {
  glfwPollEvents();
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Swap the screen buffers
  glfwSwapBuffers(window);
}

bool WindowManager::shouldClose() { return glfwWindowShouldClose(window); }
