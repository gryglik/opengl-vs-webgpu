#pragma once
#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
  Window(int width, int height, const std::string &title);

  void initWindow();
  void pollEvents();
  void swapBuffers();

  bool shouldClose();

  GLFWwindow *getNativeWindow() { return window; }

private:
  int width;
  int height;
  std::string title;

  GLFWwindow *window = nullptr;
};
