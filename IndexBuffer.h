#pragma once

class IndexBuffer
{
private:
  unsigned int m_RendererId;
  unsigned int m_Length;
public:
  IndexBuffer(const unsigned int* data, unsigned int length);
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;

  inline unsigned int GetLength() const { return m_Length; };
};