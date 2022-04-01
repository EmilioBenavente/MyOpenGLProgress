#version 330 core

in vec4 VertexColor;

uniform vec4 UniColor;

out vec4 FragColor;

void main()
{
  FragColor = VertexColor - (UniColor - vec4(0.5));
}
