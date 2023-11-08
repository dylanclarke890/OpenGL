#include <GL/glew.h> // Include before GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// How to draw a triangle in Legacy OpenGL.
static void LegacyOpenGL_DrawTriangle()
{
  glBegin(GL_TRIANGLES);
  glVertex2f(-0.5f, -0.5f);
  glVertex2f(0.0f, 0.5f);
  glVertex2f(0.5f, -0.5f);
  glEnd();
}

void InitTriangleVertexBuffer()
{
  // Defining a triangle to draw later in Modern OpenGL.
  float positions[6] = {
    -0.5f, -0.5,
    0.0f, 0.5f,
    0.5f, -0.5f
  };

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

  // Set up and enable vertex attributes.
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);
  glEnableVertexAttribArray(0);
}

// Init GLEW. Should be called after creating a valid OpenGL context.
static void InitGlew()
{
  if (glewInit() == GLEW_OK)
  {
    std::cout << "Initialised GLEW - " << glGetString(GL_VERSION) << std::endl;
  }
  else
  {
    std::cout << "Error initialising glew!" << std::endl;
  }
}

static GLFWwindow* InitOpenGL()
{
  // Initialize the OpenGL library.
  if (glfwInit() == GLFW_FALSE)
    return nullptr;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a windowed mode window and it's OpenGL context
  GLFWwindow* window;
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return nullptr;
  }

  // Create the OpenGL context.
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  InitGlew();

  return window;
}

struct ShaderProgramSource 
{
  std::string VertexSource;
  std::string FragmentSource;
};

static std::stringstream ParseFile(const std::string& filepath)
{
  std::ifstream filestream(filepath);

  std::string line;
  std::stringstream resultstream;

  while (getline(filestream, line))
  {
    resultstream << line << "\n";
  }

  return resultstream;
}

static ShaderProgramSource ParseShader(const std::string& filename)
{
  std::string vertexSource = ParseFile(filename + ".vert").str();
  std::string fragmentSource = ParseFile(filename + ".frag").str();
  return { vertexSource, fragmentSource };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
  unsigned int id = glCreateShader(type);
  
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);

    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    std::cout << message << std::endl;
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const ShaderProgramSource& source)
{
  OpenGLCall(unsigned int program = glCreateProgram());
  OpenGLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource));
  OpenGLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource));

  OpenGLCall(glAttachShader(program, vs));
  OpenGLCall(glAttachShader(program, fs));
  
  OpenGLCall(glLinkProgram(program));
  OpenGLCall(glValidateProgram(program));

  OpenGLCall(glDeleteShader(vs));
  OpenGLCall(glDeleteShader(fs));

  return program;
}

int main(void)
{
  GLFWwindow* window = InitOpenGL();
  assert(window);

  {
    // Defining square positions to draw later in Modern OpenGL.
    float positions[] = {
      -0.5f, -0.5f, // 0
      0.5f, -0.5f,  // 1
      0.5f, 0.5f,   // 2
      -0.5f, 0.5f   // 3
    };

    // Using an index buffer to avoid storing data for the same vertex multiple times
    unsigned int indices[] = {
      0, 1, 2, // Indices of positions to use for first triangle
      2, 3, 0  // Indices of positions to use for second triangle
    };

    // Create and bind the vertex array
    unsigned int vao;
    OpenGLCall(glGenVertexArrays(1, &vao));
    OpenGLCall(glBindVertexArray(vao));

    VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));

    // Set up and enable vertex attributes.
    OpenGLCall(glEnableVertexAttribArray(0));
    OpenGLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0));

    IndexBuffer indexBuffer(indices, 6);

    ShaderProgramSource shaderSource = ParseShader("Triangle");
    unsigned int shaderProgram = CreateShader(shaderSource);

    OpenGLCall(glUseProgram(0));
    OpenGLCall(glBindVertexArray(0));
    OpenGLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OpenGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    OpenGLCall(int location = glGetUniformLocation(shaderProgram, "u_Color"));
    assert(location != -1);

    float red = 0.0f;
    float increment = 0.05f;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
      // Render here
      OpenGLCall(glClear(GL_COLOR_BUFFER_BIT));

      OpenGLCall(glUseProgram(shaderProgram));
      OpenGLCall(glUniform4f(location, red, 0.3f, 0.8f, 1.0f));
      OpenGLCall(glBindVertexArray(vao));

      OpenGLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

      if (abs(red) >= 1)
        increment *= -1;
      red += increment;

      // Swap front and back buffers
      glfwSwapBuffers(window);

      // Poll for and process events
      glfwPollEvents();
    }

    OpenGLCall(glDeleteProgram(shaderProgram));
  }

  glfwTerminate();
  return 0;
}