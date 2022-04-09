#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct MaterialStruct
{
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
};

uniform MaterialStruct Material;

void main()
{    
    FragColor = mix(texture(Material.texture_diffuse1, TexCoords), texture(Material.texture_specular1, TexCoords), 0.5);
}