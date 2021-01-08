#version 420 core

out vec4 FragColor;

in vec2 TexCoords1;

uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoords1);
}
