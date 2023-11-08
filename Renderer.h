#pragma once
#include <GL/glew.h>

#include <cassert>

#define OpenGLCall(function)\
  OpenGL_ClearErrors();\
  function;\
  assert(OpenGL_LogCall(#function, __FILE__, __LINE__));

static void OpenGL_ClearErrors();
static bool OpenGL_LogCall(const char* function, const char* file, int line);