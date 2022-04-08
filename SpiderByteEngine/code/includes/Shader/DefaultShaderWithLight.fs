#version 330 core

in vec4 VertexColor;
in vec3 Normals;
in vec2 TexCoords;
in vec3 FragWorldPos;

uniform vec4 UniColor;
uniform sampler2D UniTexCoords;
uniform sampler2D UniTexCoords2;
uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 CameraPos;

struct MaterialStruct
{
  vec3 Ambient;
  vec3 Diffuse;
  vec3 Specular;

  float Shininess;
};
uniform MaterialStruct Material;

struct LightIntensityStruct
{
  vec3 Position;

  vec3 Ambient;
  vec3 Diffuse;
  vec3 Specular;
};
uniform LightIntensityStruct Light;


out vec4 FragColor;

void main()
{
  vec4 PreLight = vec4(ObjectColor, 1.0f) * mix(texture(UniTexCoords, TexCoords), texture(UniTexCoords2, -TexCoords), 0.5) * VertexColor;

  vec4 Ambient = vec4(Light.Ambient * Material.Ambient, 1.0);  

  vec3 Normal = normalize(Normals);
  vec3 LightDir = normalize(Light.Position - FragWorldPos);
  float DiffuseAngle = max(dot(Normal, LightDir), 0.0);
  vec4 Diffuse = vec4((DiffuseAngle * Material.Diffuse) * Light.Diffuse, 1.0);

  vec3 ViewDir = normalize(CameraPos - FragWorldPos);
  vec3 ReflectDir = reflect(-LightDir, Normal);
  float SpecularAngle = pow(max(dot(ViewDir, ReflectDir), 0.0), Material.Shininess);
  vec4 Specular = vec4((Material.Specular * SpecularAngle) * Light.Specular, 1.0);
  
  vec4 ResultColor = (Specular + Ambient + Diffuse) * PreLight;
  
  FragColor = ResultColor;
}
