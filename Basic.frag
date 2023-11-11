#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 v_TextureCoords;

void main()
{
	vec4 textureColor = texture(u_Texture, v_TextureCoords);
	color = textureColor;
};