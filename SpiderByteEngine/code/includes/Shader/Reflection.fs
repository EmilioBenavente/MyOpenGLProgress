#version 330 core

in vec3 Normals;
in vec3 Positions;

out vec4 FragColor;

uniform vec3 CameraPos;
uniform samplerCube CubeMap;

void main()
{
  // float Ratio = 1.0 / 2.42;
  // vec3 I = normalize(Positions - CameraPos);
  // vec3 R = refract(I, normalize(Normals), Ratio);
  vec3 I = normalize(Positions - CameraPos);
  vec3 R = reflect(I, normalize(Normals));

  FragColor = vec4(texture(CubeMap,R).rgb, 1.0);
}
