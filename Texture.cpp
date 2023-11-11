#include "ThirdParty/stb_image/stb_image.h"

#include "Texture.h"

Texture::Texture(const std::string& filepath)
  : m_RendererId(0), m_FilePath(filepath), m_LocalBuffer(nullptr), 
    m_Width(0), m_Height(0), m_BytesPerPixel(0)
{
  stbi_set_flip_vertically_on_load(1);
  m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BytesPerPixel, 4);

  OpenGLCall(glGenTextures(1, &m_RendererId));
  OpenGLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

  OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  OpenGLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

  if (m_LocalBuffer)
  {
    stbi_image_free(m_LocalBuffer);
  }
  else
  {
    std::cout << "\nError: Failed to load texture" << std::endl;
    std::cout << stbi_failure_reason() << std::endl;
    __debugbreak();
  }
}

Texture::~Texture()
{
  OpenGLCall(glDeleteTextures(1, &m_RendererId));
}

void Texture::Bind(unsigned int slot) const
{
  OpenGLCall(glActiveTexture(GL_TEXTURE0 + slot));
  OpenGLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

void Texture::Unbind() const
{
  OpenGLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
