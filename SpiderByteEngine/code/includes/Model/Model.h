#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Shader/Shader.h"
#include "../Mesh/Mesh.h"


class Model
{
 public:
  Model(char* Path);    
  void Draw(Shader& shader);

 private:
  std::vector<Mesh> Meshes;
  std::vector<Texture> Textures_Loaded;
  std::string Directory;

  void LoadModel(std::string path);
  void ProcessNode(aiNode* Node, const aiScene* Scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* Scene);
  std::vector<Texture> LoadMatTextures(
		        aiMaterial* Mat, aiTextureType Type, std::string TypeName);

  unsigned int TextureFromFile(const char* path,
			       const std::string&
			       directory
			       ,bool gamma);
};

#endif
