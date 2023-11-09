#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
  OpenGLCall(glGenVertexArrays(1, &m_RendererId));
  OpenGLCall(glBindVertexArray(m_RendererId));
}

VertexArray::~VertexArray()
{
  OpenGLCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
  Bind();
  buffer.Bind();
  const auto& elements = layout.GetElements();
  unsigned int offset = 0;

  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& element = elements[i];

    // Set up and enable vertex attributes.
    OpenGLCall(glEnableVertexAttribArray(i));
    OpenGLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
}

void VertexArray::Bind() const
{
  OpenGLCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const
{
  OpenGLCall(glBindVertexArray(0));
}
