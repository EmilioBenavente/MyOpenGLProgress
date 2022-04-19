#version 330 core

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube Cubemap;
uniform vec4 TextureMult;


void main()
{
  FragColor = TextureMult + texture(Cubemap, TexCoords);
}
