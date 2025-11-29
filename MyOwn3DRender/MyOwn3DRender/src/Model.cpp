#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);

}

void Model::loadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
}

//void Model::processNode(aiNode* node, const aiScene* scene)
//{
//}

//Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
//{
//	return  Mesh(vector<Vertex> vertices(), vector<unsigned int> indices, vector<Texture> textures);
//}

//vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
//{
//	return vector<Texture>();
//}
