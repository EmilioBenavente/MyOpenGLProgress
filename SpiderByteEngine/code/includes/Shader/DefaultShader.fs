#version 330 core

in vec4 VertexColor;
in vec2 TexCoords;

uniform vec4 UniColor;
uniform sampler2D UniTexCoords;
uniform sampler2D UniTexCoords2;

out vec4 FragColor;

void main()
{
  FragColor = mix(texture(UniTexCoords, TexCoords), texture(UniTexCoords2, -TexCoords), 0.5) * VertexColor;
}
