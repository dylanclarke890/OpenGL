#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoords;

out vec2 v_TextureCoords;

uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
  gl_Position = u_ModelViewProjectionMatrix * position;
  v_TextureCoords = textureCoords;
};