#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TestTexture2D.h"

#include "Renderer.h"

namespace test
{
  Texture2D::Texture2D()
    : m_TranslationA(glm::vec3(200.0f, 200.0f, 0.0f)), 
      m_TranslationB(glm::vec3(400.0f, 200.0f, 0.0f)),
      m_Projection(glm::ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, -1.0f, 1.0f)),
      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
  {
    // Defining square positions to draw later in Modern OpenGL.
    float positions[] = {
      -50.0f, -50.0f, 0.0f, 0.0f,  // 0
       50.0f, -50.0f, 1.0f, 0.0f,  // 1
       50.0f,  50.0f, 1.0f, 1.0f,  // 2
      -50.0f,  50.0f, 0.0f, 1.0f   // 3
    };

    // Using an index buffer to avoid storing data for the same vertex multiple times
    unsigned int indices[] = {
      0, 1, 2, // Indices of positions to use for first triangle
      2, 3, 0  // Indices of positions to use for second triangle
    };
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
    
    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

    m_Shader = std::make_unique<Shader>("src/resources/Basic.vert", "src/resources/Basic.frag");
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Texture", 0);
    m_Texture = std::make_unique<Texture>("src/resources/crazy-love.png");

    OpenGLCall(glEnable(GL_BLEND));
    OpenGLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  }

  Texture2D::~Texture2D()
  {
    OpenGLCall(glDisable(GL_BLEND));
  }

  void Texture2D::OnUpdate(float deltatime)
  {
  }

  void Texture2D::OnRender()
  {
    OpenGLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    OpenGLCall(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;
    m_Texture->Bind();

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
      glm::mat4 mvp = m_Projection * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);
      renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
      glm::mat4 mvp = m_Projection * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);
      renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }
  }

  void Texture2D::OnImGuiRender()
  {
    ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, WINDOW_WIDTH);
    ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, WINDOW_WIDTH);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }
}