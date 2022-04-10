#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Norm;
layout (location = 2) in vec3 Color;
layout (location = 3) in vec2 UV;

uniform float Offset;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 TexCoords;
out vec3 Normals;
out vec3 FragWorldPos;
out vec3 OutColor;

out vec4 InPos;

void main()
{
  gl_Position = Projection * View * Model * vec4(Pos, 1.0);
  InPos = gl_Position;
  TexCoords = UV;
  OutColor = Color;
  Normals = mat3(transpose(inverse(Model))) * Norm;
  FragWorldPos = vec3(Model * vec4(Pos, 1.0));
}
