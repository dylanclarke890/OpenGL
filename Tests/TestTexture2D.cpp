#include <imgui/imgui.h>

#include "../Renderer.h"
#include "TestTexture2D.h"

namespace test
{
  Texture2D::Texture2D()
  {
    // Defining square positions to draw later in Modern OpenGL.
    float positions[] = {
      -50.0f, -50.0f, 0.0f, 0.0f, // 0
       50.0f, -50.0f, 1.0f, 0.0f, // 1
       50.0f,  50.0f, 1.0f, 1.0f,  // 2
      -50.0f,  50.0f, 0.0f, 1.0f   // 3
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

    glm::mat4 projection = glm::ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    Shader shader("Basic.vert", "Basic.frag");
    shader.Bind();

    Texture texture("crazy-love.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    shader.Unbind();
    vertexArray.Unbind();
    indexBuffer.Unbind();
    vertexBuffer.Unbind();

    glm::vec3 translationA(200.0f, 200.0f, 0.0f);
    glm::vec3 translationB(400.0f, 200.0f, 0.0f);
  }

  Texture2D::~Texture2D()
  {
  }

  void Texture2D::OnUpdate(float deltatime)
  {
  }

  void Texture2D::OnRender()
  {
    OpenGLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    OpenGLCall(glClear(GL_COLOR_BUFFER_BIT));

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
      glm::mat4 mvp = projection * view * model;
      shader.Bind();
      shader.SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);
      renderer.Draw(vertexArray, indexBuffer, shader);
    }

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
      glm::mat4 mvp = projection * view * model;
      shader.Bind();
      shader.SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);
      renderer.Draw(vertexArray, indexBuffer, shader);
    }
  }

  void Texture2D::OnImGuiRender()
  {
  }
}