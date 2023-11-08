#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
  OpenGLCall(glGenBuffers(1, &m_RendererId));
  OpenGLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
  OpenGLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
  OpenGLCall(glDeleteBuffers(1, &m_RendererId));
}

void VertexBuffer::Bind() const
{
  OpenGLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::Unbind() const
{
  OpenGLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
