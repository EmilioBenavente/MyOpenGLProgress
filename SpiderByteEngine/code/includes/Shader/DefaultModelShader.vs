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

void main()
{
  gl_Position = Projection * View * Model * vec4(Pos, 1.0);
  TexCoords = UV;
  Normals = mat3(transpose(inverse(Model))) * Norm;
  FragWorldPos = vec3(Model * vec4(Pos, 1.0));
}


#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}