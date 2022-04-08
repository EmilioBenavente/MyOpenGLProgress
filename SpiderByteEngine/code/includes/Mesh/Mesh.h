#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Shader/Shader.h"

#include <vector>
#include <string>

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture
{
  unsigned int ID;
  std::string Type;
  std::string Path;
};

class Mesh
{
 public:
  std::vector<Vertex> Vertices;
  std::vector<unsigned int> Indices;
  std::vector<Texture> Textures;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int>
       indices, std::vector<Texture> textures);

  void Draw(Shader& shader);

 private:
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;

  void SetUpMesh();  
};
#endif
