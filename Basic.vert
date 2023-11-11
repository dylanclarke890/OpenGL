#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoords;

out vec2 v_TextureCoords;

void main()
{
  gl_Position = position;
  v_TextureCoords = textureCoords;
};