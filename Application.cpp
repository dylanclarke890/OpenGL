#include <GL/glew.h> // Include before GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

#define OpenGLCall(function)\
  OpenGL_ClearErrors();\
  function;\
  assert(OpenGL_LogCall(#function, __FILE__, __LINE__));

static void OpenGL_ClearErrors() 
{
  while (glGetError() != GL_NO_ERROR);
}

static bool OpenGL_LogCall(const char* function, const char* file, int line) 
{
  while (GLenum error = glGetError())
  {
    std::cout << "[ERROR] [OPENGL]: " << error << std::endl;
    std::cout << "file: " << file << " line: " << line << std::endl;
    return false;
  }
  return true;
}

// How to draw a triangle in Legacy OpenGL.
static void LegacyOpenGL_DrawTriangle()
{
  glBegin(GL_TRIANGLES);
  glVertex2f(-0.5f, -0.5f);
  glVertex2f(0.0f, 0.5f);
  glVertex2f(0.5f, -0.5f);
  glEnd();
}

static GLFWwindow* InitOpenGL()
{
  // Initialize the OpenGL library.
  if (glfwInit() == GLFW_FALSE)
    return nullptr;

  // Create a windowed mode window and it's OpenGL context
  GLFWwindow* window;
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return nullptr;
  }

  // Create the OpenGL context.
  OpenGLCall(glfwMakeContextCurrent(window));
  
  return window;
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

struct ShaderProgramSource 
{
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
  std::ifstream stream(filepath);

  enum class ShaderType
  {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while(getline(stream, line)) 
  {
    if (line.find("#shader") != std::string::npos) 
    {
      if (line.find("vertex") != std::string::npos)
      {
        type = ShaderType::VERTEX;
      }
      else if (line.find("fragment") != std::string::npos)
      {
        type = ShaderType::FRAGMENT;

      }
    }
    else if (type != ShaderType::NONE)
    {
      ss[(int)type] << line << "\n";
    }
  }

  return { ss[0].str(), ss[1].str() };
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
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
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

void InitSquareVertexBuffer()
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

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

  // Set up and enable vertex attributes.
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);
  glEnableVertexAttribArray(0);

  unsigned int ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}


static bool s_IsDrawingTriangle = false;
int main(void)
{
  OpenGLCall(GLFWwindow* window = InitOpenGL());
  if (!window)
    return -1;

  InitGlew();
  OpenGLCall(
    if (s_IsDrawingTriangle)
      InitTriangleVertexBuffer();
    else
      InitSquareVertexBuffer();
  );

  ShaderProgramSource shaderSource = ParseShader("Basic.shader");
  unsigned int shaderProgram = CreateShader(shaderSource);

  OpenGLCall(glUseProgram(shaderProgram));

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window))
  {
    // Render here
    OpenGLCall(glClear(GL_COLOR_BUFFER_BIT));

    if (s_IsDrawingTriangle) 
    {
      OpenGLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
    }
    else
    {
      OpenGLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));
    }

    // Swap front and back buffers
    OpenGLCall(glfwSwapBuffers(window));

    // Poll for and process events
    OpenGLCall(glfwPollEvents());
  }

  OpenGLCall(glDeleteProgram(shaderProgram));
  OpenGLCall(glfwTerminate());

  return 0;
}