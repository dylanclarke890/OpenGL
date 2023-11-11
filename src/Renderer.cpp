#include "Renderer.h"

void Renderer::Clear() const
{
  OpenGLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
  shader.Bind();
  indexBuffer.Bind();
  vertexArray.Bind();

  OpenGLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetLength(), GL_UNSIGNED_INT, nullptr));

}
