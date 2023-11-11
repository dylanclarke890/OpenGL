#include <GL/glew.h> // Include before GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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

int main(void)
{
  GLFWwindow* window = InitOpenGL();
  assert(window);

  {
    // Defining square positions to draw later in Modern OpenGL.
    float positions[] = {
      -0.5f, -0.5f, 0.0f, 0.0f, // 0
      0.5f, -0.5f, 1.0f, 0.0f, // 1
      0.5f, 0.5f, 1.0f, 1.0f,   // 2
      -0.5f, 0.5f, 0.0f, 1.0f   // 3
    };

    // Using an index buffer to avoid storing data for the same vertex multiple times
    unsigned int indices[] = {
      0, 1, 2, // Indices of positions to use for first triangle
      2, 3, 0  // Indices of positions to use for second triangle
    };

    OpenGLCall(glEnable(GL_BLEND));
    OpenGLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray vertexArray;
    VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    vertexArray.AddBuffer(vertexBuffer, layout);
    IndexBuffer indexBuffer(indices, 6);

    Shader shader("Basic.vert", "Basic.frag");
    shader.Bind();

    Texture texture("crazy-love.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    shader.Unbind();
    vertexArray.Unbind();
    indexBuffer.Unbind();
    vertexBuffer.Unbind();

    Renderer renderer;

    float red = 0.0f;
    float increment = 0.05f;
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
      renderer.Clear();

      shader.Bind();
      shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

      renderer.Draw(vertexArray, indexBuffer, shader);

      if (abs(red) >= 1)
        increment *= -1;
      red += increment;

      // Swap front and back buffers
      glfwSwapBuffers(window);

      // Poll for and process events
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return 0;
}