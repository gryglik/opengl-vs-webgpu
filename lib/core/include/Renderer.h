#pragma once

#include "Window.h"
class Renderer {
public:
  Renderer(Window &window) : window(window) {}
  virtual ~Renderer() = default;
  virtual void init() = 0;
  virtual void render() = 0;

protected:
  Window &window;
};
