#version 330 core

in vec4 VertexColor;

uniform vec4 UniColor;

out vec4 FragColor;

void main()
{
  FragColor = vec4(0.5f, 0.5f, 0.0f, 1.0f);
}
