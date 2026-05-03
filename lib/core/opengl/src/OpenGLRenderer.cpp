#include "OpenGLRenderer.h"
#include "glad/gl.h"

void OpenGLRenderer::init() {
  // -- Prepare shaders
  prepareVertexShader();
  prepareFragmentShader();

  // -- Linking shaders
  program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  // -- Prepare buffers
  prepareBuffers();
}

void OpenGLRenderer::prepareVertexShader() {
  // TODO: move to external files
  const char *vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec2 aPos;\n"
                                   "layout (location = 1) in vec3 aCol;\n"
                                   "uniform float intensity;\n"
                                   "out vec3 vCol;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   vCol = aCol;\n"
                                   "   gl_Position = vec4(aPos, 0.0, 1.0);\n"
                                   "}\0";

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
}

void OpenGLRenderer::prepareFragmentShader() {
  // TODO: moce to external files
  const char *fragmentShaderSource =
      "#version 330 core\n"
      "in vec3 vCol;"
      "out vec4 FragColor;\n"
      "uniform float intensity;\n"
      "void main()\n"
      "{\n"
      "   FragColor = intensity * vec4(vCol, 1.0f);\n"
      "}\n\0";

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
}

void OpenGLRenderer::prepareBuffers() {
  std::vector<glm::vec2> positions;
  positions.push_back(glm::vec2(-0.5f, -0.5f));
  positions.push_back(glm::vec2(0.5f, -0.5f));
  positions.push_back(glm::vec2(0.0f, 0.5f));

  std::vector<glm::vec3> colors;
  colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
  colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
  colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

  std::vector<Vertex> bufferData;
  for (int i = 0; i < positions.size(); ++i)
    bufferData.push_back({positions[i], colors[i]});

  // VAO - Vertex Attributes Object
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // VBO - Vertex Buffer Object
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * bufferData.size(),
               bufferData.data(), GL_STATIC_DRAW);

  // -- VAO - Vector Attributes Object
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)sizeof(glm::vec2));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void OpenGLRenderer::render() {
  glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
