#version 330 core
out vec4 FragColor;

in vec4 VertexColor;
in vec2 TexCoords;

struct MaterialStruct
{
  sampler2D Diffuse1;
  sampler2D Specular1;
};

uniform MaterialStruct Material;

void main()
{    
    FragColor = texture(Material.Diffuse1, TexCoords);
}