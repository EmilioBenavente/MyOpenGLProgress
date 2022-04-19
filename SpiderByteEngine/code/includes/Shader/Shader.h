#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

class Shader
{
 public:
  unsigned int PROGRAM_ID;
  Shader(const char* VertPath, const char* FragPath, const char* GeometryPath);
  void Use();
  void SetBool(char* Name, bool A);
  void SetBool2(char* Name, bool A, bool B);
  void SetBool3(char* Name, bool A, bool B, bool C);
  void SetBool4(char* Name, bool A, bool B, bool C, bool D);
  
  void SetInt(char* Name, int A);
  void SetInt2(char* Name, int A, int B);
  void SetInt3(char* Name, int A, int B, int C);
  void SetInt4(char* Name, int A, int B, int C, int D);
  
  void SetFloat(char* Name, float A);
  void SetFloat2(char* Name, float A, float B);
  void SetFloat3(char* Name, float A, float B, float C);
  void SetFloat4(char* Name, float A, float B, float C, float D);

  void SetVec2(char* Name, glm::vec2 Value);
  void SetVec3(char* Name, glm::vec3 Value);
  void SetVec4(char* Name, glm::vec4 Value);

  void SetMat2(char* Name, glm::mat2 Value);
  void SetMat3(char* Name, glm::mat3 Value);
  void SetMat4(char* Name, glm::mat4 Value);

  void SetUniformBlockBinding(char* Name, int Value);
};
#endif
