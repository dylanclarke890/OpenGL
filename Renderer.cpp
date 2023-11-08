#include "Renderer.h"

#include <iostream>

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