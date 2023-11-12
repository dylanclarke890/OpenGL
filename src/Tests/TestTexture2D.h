#pragma once

#include <memory>

#include "Test.h"

#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace test
{
  class Texture2D: public Test
  {
  private:
    glm::vec3 m_TranslationA, m_TranslationB;
    glm::mat4 m_Projection, m_View;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;

  public:
    Texture2D();
    ~Texture2D();

    void OnUpdate(float deltatime);
    void OnRender();
    void OnImGuiRender();
  };
}