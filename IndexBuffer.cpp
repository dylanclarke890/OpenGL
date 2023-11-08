#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int length)
  : m_Length(length)
{
  OpenGLCall(glGenBuffers(1, &m_RendererId));
  OpenGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
  OpenGLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof (unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
  OpenGLCall(glDeleteBuffers(1, &m_RendererId));
}

void IndexBuffer::Bind() const
{
  OpenGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void IndexBuffer::Unbind() const
{
  OpenGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
