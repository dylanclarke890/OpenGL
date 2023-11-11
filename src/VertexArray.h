#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
  unsigned int m_RendererId;
public:
  VertexArray();
  ~VertexArray();

  void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);

  void Bind() const;
  void Unbind() const;
};