#version 330 core

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube Cubemap;

void main()
{
  FragColor = texture(Cubemap, TexCoords);
}
