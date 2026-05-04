#include "OpenGLRenderer.h"
#include "WebGPURenderer.h"
#include "Window.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

template <class R> class Application {
public:
  Application(int width, int height, std::string const &title)
      : window(width, height, title), renderer(std::make_unique<R>(window)) {}
  void init();
  void run();

private:
  Window window;
  std::unique_ptr<Renderer> renderer;
};

template <class R> void Application<R>::init() {
  window.initWindow();
  renderer->init();
}

template <class R> void Application<R>::run() {
  while (!window.shouldClose()) {
    renderer->render();

    window.swapBuffers();
    window.pollEvents();
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2)
    std::cout << "Usage: ./app [opengl|webgpu]\n";

  if (strcmp(argv[1], "opengl") == 0) {
    Application<OpenGLRenderer> app(1200, 800, "Hello app!");
    app.init();
    app.run();
  } else if (strcmp(argv[1], "webgpu") == 0) {
    Application<WebGPURenderer> app(1200, 800, "Hello app!");
    app.init();
    app.run();
  } else
    throw std::runtime_error("Invalid renderer.");

  return 0;
}
