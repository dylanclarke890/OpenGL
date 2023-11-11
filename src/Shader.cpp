
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

#include "Renderer.h"
#include "Shader.h"

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
  : m_VertexFilePath(vertexFile), m_FragmentFilePath(fragmentFile), m_RendererId(0)
{
  ShaderProgramSource shaderSource = ParseShader();
  m_RendererId = CreateShader(shaderSource);
}

Shader::~Shader()
{
  OpenGLCall(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const
{
  OpenGLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
  OpenGLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
  OpenGLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
  OpenGLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
  OpenGLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
  OpenGLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  OpenGLCall(int location = glGetUniformLocation(m_RendererId, name.c_str()));
  if (location == -1)
    std::cout << "[WARNING] [OPENGL]: Uniform '" << name << "' doesn't exist!" << std::endl;
  
  m_UniformLocationCache[name] = location;
  
  return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
  unsigned int id = glCreateShader(type);

  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);

    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    std::cout << message << std::endl;
    return 0;
  }

  return id;
}

unsigned int Shader::CreateShader(const ShaderProgramSource& source)
{
  OpenGLCall(unsigned int program = glCreateProgram());
  OpenGLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource));
  OpenGLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource));

  OpenGLCall(glAttachShader(program, vs));
  OpenGLCall(glAttachShader(program, fs));

  OpenGLCall(glLinkProgram(program));
  OpenGLCall(glValidateProgram(program));

  OpenGLCall(glDeleteShader(vs));
  OpenGLCall(glDeleteShader(fs));

  return program;
}

std::stringstream Shader::ParseFile(const std::string& filepath)
{
  std::ifstream filestream(filepath);

  std::string line;
  std::stringstream resultstream;

  while (getline(filestream, line))
  {
    resultstream << line << "\n";
  }

  return resultstream;
}

ShaderProgramSource Shader::ParseShader()
{
  std::string vertexSource = ParseFile(m_VertexFilePath).str();
  std::string fragmentSource = ParseFile(m_FragmentFilePath).str();
  return { vertexSource, fragmentSource };
}