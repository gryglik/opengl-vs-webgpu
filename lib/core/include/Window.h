#pragma once
#include "glad/gl.h"

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

class Window {
public:
  Window(unsigned int width, unsigned int height, const std::string &title);

  void initWindow();
  void pollEvents();
  void swapBuffers();

  bool shouldClose();

  unsigned int getWidth() const { return this->width; }
  unsigned int getHeight() const { return this->height; }

  GLFWwindow *getNativeWindow() { return window; }

private:
  unsigned int width;
  unsigned int height;
  std::string title;

  GLFWwindow *window = nullptr;
};
