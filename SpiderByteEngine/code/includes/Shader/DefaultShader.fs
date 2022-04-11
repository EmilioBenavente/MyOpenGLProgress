#version 330 core

//in vec4 VertexColor;
//in vec2 TexCoords;

//uniform vec4 UniColor;
//uniform sampler2D UniTexCoords;
//uniform sampler2D UniTexCoords2;

out vec4 FragColor;

void main()
{
  FragColor = vec4(0.34, 0.62, 0.44, 1.0);
  //texture(UniTexCoords, TexCoords);
  //  FragColor = mix(texture(UniTexCoords, TexCoords), texture(UniTexCoords2, -TexCoords), 0.5) * VertexColor;
}
