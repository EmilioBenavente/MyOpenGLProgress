#version 330 core
layout (location = 0) in vec3 InPos;
layout (location = 1) in vec3 InColor;
layout (location = 2) in vec2 InUVs;

out vec2 TexCoords;

layout (std140) uniform Matrices
{
  uniform mat4 Projection;
  uniform mat4 View;
};

uniform mat4 Model;

void main()
{
  gl_Position = Projection * View * Model * vec4(InPos,1.0);
  TexCoords = InUVs;
}
