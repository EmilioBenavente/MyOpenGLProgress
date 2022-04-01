#ifndef SHADER_H
#define SHADER_H

class Shader
{
 public:
  unsigned int PROGRAM_ID;
  Shader(const char* VertPath, const char* FragPath);
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

  //@TODO(Emilio): These functions will be added once we include
  //the GLM Library
  /* void SetVec2(); */
  /* void SetVec3(); */
  /* void SetVec4(); */

  /* void SetMat2(); */
  /* void SetMat3(); */
  /* void SetMat4();   */
};
#endif
