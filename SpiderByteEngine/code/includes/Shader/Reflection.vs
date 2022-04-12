#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normal;

out vec3 Positions;
out vec3 Normals;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
  Normals = mat3(transpose(inverse(Model))) * Normal;
  Positions = vec3(Model * vec4(Pos, 1.0)); 
  gl_Position = Projection * View * vec4(Positions, 1.0);
}
