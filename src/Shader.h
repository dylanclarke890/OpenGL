#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader
{
private:
  std::string m_VertexFilePath;
  std::string m_FragmentFilePath;
  unsigned int m_RendererId;
  std::unordered_map<std::string, int> m_UniformLocationCache;
public:
  Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
  ~Shader();

  void Bind() const;
  void Unbind() const;


  void SetUniform1f(const std::string& name, float value);
  void SetUniform1i(const std::string& name, int value);

  void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
  void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
  int GetUniformLocation(const std::string& name);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  unsigned int CreateShader(const ShaderProgramSource& source);
  ShaderProgramSource ParseShader();
  std::stringstream ParseFile(const std::string& filepath);
};