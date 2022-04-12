#version 330 core

layout (location = 0) in vec3 Pos;

out vec3 TexCoords;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
  TexCoords = Pos;
  vec4 Position = Projection * View * vec4(Pos, 1.0);
  gl_Position = Position.xyww;
}
