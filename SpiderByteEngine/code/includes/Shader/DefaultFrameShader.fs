#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D FrameTex;

float ConstK = 1.0 / 30.0;
vec2 NewTextureCoords[9] = vec2[](
   vec2(-ConstK, ConstK),
   vec2(0.0, ConstK),
   vec2(ConstK, ConstK),
   vec2(-ConstK, 0.0),
   vec2(0.0, 0.0),
   vec2(ConstK, 0.0),
   vec2(-ConstK, -ConstK),
   vec2(0.0, -ConstK),
   vec2(ConstK, -ConstK)        
     );

float Kernel[9] = float[](
   1.0/12.0, 2.0/12.0, 1.0/12.0,
   1.0/12.0, 4.0/12.0, 1.0/12.0,
   1.0/12.0, 2.0/12.0, 1.0/12.0
			  );


void main()
{
  vec3 SampleTexture[9];
  for(int i = 0; i < 9; ++i)
    {
      SampleTexture[i] = vec3(texture(FrameTex, TexCoords.st + NewTextureCoords[i]));
    }
  vec3 FinalTexture = vec3(0.0);
  for(int i = 0; i < 9; ++i)
    {
      FinalTexture += SampleTexture[i] * Kernel[i];
    }
  
  FragColor = vec4(FinalTexture, 1.0);
}
