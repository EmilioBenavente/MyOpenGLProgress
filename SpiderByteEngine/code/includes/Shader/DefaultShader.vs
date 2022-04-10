#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 UV;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec4 VertexColor;
out vec2 TexCoords;

void main()
{
  gl_Position = Projection * View * Model * vec4(Pos, 1.0);
  VertexColor = vec4(gl_Position.z, gl_Position.z, gl_Position.z, 1.0);
  TexCoords = UV;
}
