#include "Window.h"

class Application {
public:
  Application() : window(800, 600, "Hello world!") {}
  void init();
  void run();

private:
  WindowManager window;
};

void Application::init() { window.initWindow(); }

void Application::run() {
  while (!window.shouldClose()) {
    window.pollEvents();
  }
}

int main() {
  Application app;
  app.init();
  app.run();
}
