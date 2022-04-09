#include <stdio.h>
#include <vector>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Textures/stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include "../Mesh/Mesh.h"

Model::Model(char* Path)
{
  LoadModel(Path);
}

void Model::LoadModel(std::string path)
{
  Assimp::Importer Importer;
  const aiScene* Scene = Importer.ReadFile(path,
					   aiProcess_Triangulate);// |
	         // aiProcess_FlipUVs);
  if(!Scene || Scene->mFlags &
     AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
      printf("SPI_ERROR -> Unable to load model with"
             "given path\n%s\nErr From Lib:\n%s\n",
	     path.c_str(), Importer.GetErrorString());
      return;
    }

  Directory = path.substr(0, path.find_last_of('/'));

  ProcessNode(Scene->mRootNode, Scene);
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
  std::string filename = std::string(path);
    filename = directory + '/' + filename;
    
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        stbi_image_free(data);
    }

    return textureID;
}

void Model::ProcessNode(aiNode* Node,
			const aiScene* Scene)
{
  for(unsigned int i = 0; i < Node->mNumMeshes; ++i)
    {
      aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
      Meshes.push_back(ProcessMesh(Mesh, Scene));
    }

  for(unsigned int i = 0; i < Node->mNumChildren; ++i)
    {
      ProcessNode(Node->mChildren[i], Scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* Scene)
{
  std::vector<Vertex> Vertices;
  std::vector<unsigned int> Indices;
  std::vector<Texture> Textures;

  for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
      Vertex vertex;

      glm::vec3 Vector;
      Vector.x = mesh->mVertices[i].x;
      Vector.y = mesh->mVertices[i].y;
      Vector.z = mesh->mVertices[i].z;
      vertex.Position = Vector;

      Vector.x = mesh->mNormals[i].x;
      Vector.y = mesh->mNormals[i].y;
      Vector.z = mesh->mNormals[i].z;
      vertex.Normal = Vector;

      if(mesh->mTextureCoords[0])
	{
	  glm::vec2 UVs;
	  UVs.x = mesh->mTextureCoords[0][i].x;
	  UVs.y = mesh->mTextureCoords[0][i].y;

	  vertex.TexCoords = UVs;	  
	}
      else
	{
	  vertex.TexCoords = glm::vec2(0.0f, 0.0f);
	}

      Vertices.push_back(vertex);      
    }

  for(int i = 0; i < mesh->mNumFaces; ++i)
    {
      aiFace Face = mesh->mFaces[i];
      for(unsigned int j = 0; j < Face.mNumIndices;
	  j++)
	{
	  Indices.push_back(Face.mIndices[j]);
	}
    }
  
  if(mesh->mMaterialIndex >= 0)
    {
      aiMaterial* Materials =
	Scene->mMaterials[mesh->mMaterialIndex];

      std::vector<Texture> DiffuseMaps =
	LoadMatTextures(Materials,
			     aiTextureType_DIFFUSE,
			     "Diffuse");
      Textures.insert(Textures.end(),
		      DiffuseMaps.begin(),
		      DiffuseMaps.end());
      std::vector<Texture> SpecularMaps =
	LoadMatTextures(Materials,
			     aiTextureType_SPECULAR,
			     "Specular");
      Textures.insert(Textures.end(),
		      SpecularMaps.begin(),
		      SpecularMaps.end());
    }
  return Mesh(Vertices, Indices, Textures);
}

void Model::Draw(Shader& shader)
{
  for(unsigned int i = 0; i < Meshes.size(); ++i)
    {
      Meshes[i].Draw(shader);
    }
}

std::vector<Texture> Model::LoadMatTextures(
                     aiMaterial* Mat,
		     aiTextureType Type,
		     std::string TypeName)
{
  std::vector<Texture> Textures;
  for(unsigned int i = 0; i <
	Mat->GetTextureCount(Type); ++i)
    {
      aiString Str;
      Mat->GetTexture(Type, i, &Str);
      int IsNotCached = 1;
      for(unsigned int j = 0; j <
	    Textures_Loaded.size(); ++j)
	{
	  if(std::strcmp
	     (Textures_Loaded[j].Path.data(),
	      Str.C_Str()) == 0)
	    {
	      Textures.push_back(Textures_Loaded[j]);
	      IsNotCached = 0;
	      break;
	    }
	}
      if(IsNotCached)
	{
	  Texture texture;
	  texture.ID = TextureFromFile(Str.C_Str(),
				       Directory,
				       false);
	  texture.Type = TypeName;
	  texture.Path = Str.C_Str();
	  Textures.push_back(texture);
	  Textures_Loaded.push_back(texture);	  
	}
    }

  return Textures;
}

