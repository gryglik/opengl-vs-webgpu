#pragma once
#include "Renderer.h"
#include <glad/gl.h>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>
#include <vector>

class OpenGLRenderer : public Renderer {
public:
  typedef struct Vertex {
    glm::vec2 position;
    glm::vec3 colors;
  } Vertex;

  OpenGLRenderer(Window &window) : Renderer(window) {}
  void init() override;
  void render() override;

private:
  GLuint VBO;
  GLuint VAO;
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint program;

  void prepareVertexShader();
  void prepareFragmentShader();
  void prepareBuffers();
};
