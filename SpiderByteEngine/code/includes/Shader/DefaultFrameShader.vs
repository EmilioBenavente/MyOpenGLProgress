#version 330 core
layout (location = 0) in vec3 InPos;
layout (location = 1) in vec3 InColor;
layout (location = 2) in vec2 InUVs;

out vec2 TexCoords;

void main()
{
  gl_Position = vec4(InPos.xy, 0.0,1.0);
  TexCoords = InUVs;
}
