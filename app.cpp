#include "OpenGLRenderer.h"
#include "Window.h"
#include <memory>
#include <string>

class Application {
public:
  Application(int width, int height, std::string const &title)
      : window(width, height, title),
        renderer(std::make_unique<OpenGLRenderer>()) {}
  void init();
  void run();

private:
  Window window;
  std::unique_ptr<Renderer> renderer;
};

void Application::init() {
  window.initWindow();
  renderer->init();
}

void Application::run() {
  while (!window.shouldClose()) {
    renderer->render();

    window.swapBuffers();
    window.pollEvents();
  }
}

int main() {
  Application app(1200, 800, "Hello app!");
  app.init();
  app.run();
}
