#version 330 core

in vec4 VertexColor;
in vec3 Normals;
in vec2 TexCoords;
in vec3 FragWorldPos;

uniform vec4 UniColor;
uniform sampler2D UniTexCoords;
uniform sampler2D UniTexCoords2;
uniform vec3 ObjectColor;
uniform vec3 CameraPos;

struct MaterialStruct
{
  sampler2D Ambient;
  sampler2D Diffuse;
  sampler2D Specular;

  float Shininess;
};
uniform MaterialStruct Material;

struct DirectionLight
{
  vec3 Direction;

  vec3 AmbientIntensity;
  vec3 DiffuseIntensity;
  vec3 SpecularIntensity;
};
uniform DirectionLight DirLight;

struct PointLight
{
  vec3 Position;

  float ConstantK;
  float LinearK;
  float QuadK;

  vec3 AmbientIntensity;
  vec3 DiffuseIntensity;
  vec3 SpecularIntensity;
};
#define MAX_POINT_LIGHTS 4
uniform PointLight PointLights[MAX_POINT_LIGHTS];
  
struct SpotLightStruct
{
  vec3 Direction;
  vec3 Position;

  float ConstantK;
  float LinearK;
  float QuadK;
  
  vec3 AmbientIntensity;
  vec3 DiffuseIntensity;
  vec3 SpecularIntensity;

  float SpotlightCutoffRAD;
  float OuterConeCutoffRAD;
};

uniform SpotLightStruct SpotLight;

vec3 CalcDirLight(DirectionLight DirLight, vec3 Normal, vec3 ViewDir);
vec3 CalcPointLight(PointLight PointLight, vec3 Normal,
		    vec3 FragWorldPos, vec3 ViewDir);
vec3 CalcSpotLight(SpotLightStruct SpotLight, vec3 Normal, vec3 FragWorldPos, vec3 ViewDir);

  
out vec4 FragColor;

void main()
{
  vec3 Normal = normalize(Normals);
  vec3 CameraDir = normalize(CameraPos - FragWorldPos);

  
  vec3 PreLight = ObjectColor * mix(vec3(texture(UniTexCoords,
						  TexCoords)),
				    vec3(texture(UniTexCoords2,
						 TexCoords)), 0.6);
  vec3 ResultColor = ObjectColor;
   ResultColor *= CalcDirLight(DirLight, Normal, CameraDir);

  for(int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
      ResultColor += CalcPointLight(PointLights[i], Normal,
				    FragWorldPos, CameraDir);
    }
  ResultColor += CalcSpotLight(SpotLight, Normal,
			       FragWorldPos, CameraDir);
  
  FragColor = vec4(ResultColor, 1.0);
}


vec3 CalcDirLight(DirectionLight DirLight, vec3 Normal, vec3 ViewDir)
{
  vec3 LightDir = normalize(-DirLight.Direction);
  vec3 ReflectDir = reflect(-LightDir, Normal);  

  float DiffRAD = max(dot(Normal, LightDir), 0.0);
  float SpecularRAD = pow(max(dot(ViewDir, ReflectDir), 0.0),
				Material.Shininess);
  
  vec3 Ambient = DirLight.AmbientIntensity
                 * vec3(texture(Material.Ambient,TexCoords));
  vec3 Diffuse = DirLight.DiffuseIntensity * DiffRAD *
                 vec3(texture(Material.Diffuse,TexCoords));
  vec3 Specular = DirLight.SpecularIntensity * SpecularRAD *
                  vec3(texture(Material.Specular, TexCoords));

  return (Ambient + Diffuse + Specular);
}

vec3 CalcPointLight(PointLight PointLight, vec3 Normal,
		    vec3 FragWorldPos, vec3 ViewDir)
{
  vec3 LightDir = normalize(PointLight.Position - FragWorldPos);
  vec3 ReflectDir = reflect(-LightDir, Normal);
  
  float DiffRAD = max(dot(Normal, LightDir), 0.0);
  float SpecRAD = pow(max(dot(ViewDir, ReflectDir),0.0),
		      Material.Shininess);
  float Distance = length(PointLight.Position - FragWorldPos);
  float Attenuation = 1.0 /(PointLight.ConstantK +
			   (PointLight.LinearK * Distance) +
			   (PointLight.QuadK * (Distance * Distance)));

  vec3 Ambient = PointLight.AmbientIntensity
                 * vec3(texture(Material.Ambient,TexCoords))
                 * Attenuation;
  vec3 Diffuse = PointLight.DiffuseIntensity * DiffRAD *
                 vec3(texture(Material.Diffuse,TexCoords))
                 * Attenuation;
  vec3 Specular = PointLight.SpecularIntensity * SpecRAD *
                  vec3(texture(Material.Specular, TexCoords))
                 * Attenuation;

  return (Ambient + Diffuse + Specular);  
}


vec3 CalcSpotLight(SpotLightStruct SpotLight, vec3 Normals,
		   vec3 FragWorldPos, vec3 ViewDir)
{
  vec3 LightDir = normalize(SpotLight.Position - FragWorldPos);
  vec3 ReflectDir = reflect(-LightDir, Normals);

  float DiffRAD = max(dot(Normals, LightDir), 0.0);
  float SpecRAD = pow(max(dot(ViewDir, ReflectDir),0.0),
		      Material.Shininess);  vec3 Normal = normalize(Normals);

  float Distance = length(SpotLight.Position - FragWorldPos);
  float Attenuation = 1.0 /(SpotLight.ConstantK +
			   (SpotLight.LinearK * Distance) +
			   (SpotLight.QuadK * (Distance * Distance)));

  float Theta = dot(LightDir, normalize(-SpotLight.Direction));
  if(Theta > SpotLight.SpotlightCutoffRAD)
    {
      float Epsilon = SpotLight.SpotlightCutoffRAD
	- SpotLight.OuterConeCutoffRAD;
      float Intensity = clamp((Theta - SpotLight.OuterConeCutoffRAD)
			      / Epsilon, 0.0, 1.0);
  
      vec3 Ambient = SpotLight.AmbientIntensity
	* vec3(texture(Material.Ambient,TexCoords))
	* Attenuation;
      vec3 Diffuse = SpotLight.DiffuseIntensity * DiffRAD *
	vec3(texture(Material.Diffuse,TexCoords))
	* Attenuation;
      vec3 Specular = SpotLight.SpecularIntensity * SpecRAD *
	vec3(texture(Material.Specular, TexCoords))
	* Attenuation;
      return (Ambient + Diffuse + Specular);    
    }
  return vec3(0.0f);
}
