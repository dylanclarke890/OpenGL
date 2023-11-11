#pragma once

#include <GL/glew.h>
#include <cassert>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define OpenGLCall(function)\
  OpenGL_ClearErrors();\
  function;\
  assert(OpenGL_LogCall(#function, __FILE__, __LINE__));

static void OpenGL_ClearErrors();
static bool OpenGL_LogCall(const char* function, const char* file, int line);

static void OpenGL_ClearErrors()
{
  while (glGetError() != GL_NO_ERROR);
}

static bool OpenGL_LogCall(const char* function, const char* file, int line)
{
  while (GLenum error = glGetError())
  {
    std::cout << "[ERROR] [OPENGL]: " << error << std::endl;
    std::cout << "file: " << file << " line: " << line << std::endl;
    return false;
  }
  return true;
}

class Renderer
{
public:
  void Clear() const;
  void Draw(const VertexArray& va, const IndexBuffer& ia, const Shader& shader) const;
};