#include <GL/glew.h> // Include before GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

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
    else
    {
      ss[(int)type] << line << "\n";
    }
  }

  return { ss[0].str(), ss[1].str() };
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
  glfwMakeContextCurrent(window);

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

int main(void)
{
  GLFWwindow* window = InitOpenGL();
  if (!window)
    return -1;

  InitGlew();

  // Setting up triangle buffers / shaders
  InitTriangleVertexBuffer();
  ShaderProgramSource shaderSource = ParseShader("Basic.shader");
  unsigned int shaderProgram = CreateShader(shaderSource);

  glUseProgram(shaderProgram);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window))
  {
    // Render here
    glClear(GL_COLOR_BUFFER_BIT);

    // Modern OpenGL triangle.
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}