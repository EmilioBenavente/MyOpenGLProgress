#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;

uniform float Offset;
out vec4 VertexColor;

void main()
{
  gl_Position = vec4(-(Pos + Offset), 1.0);
  VertexColor = vec4(Pos, 1.0);
}
