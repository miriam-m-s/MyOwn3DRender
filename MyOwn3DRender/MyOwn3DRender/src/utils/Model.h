#pragma once
#include <string>
#include"Model.h"
#include "Mesh.h"
#include"Texture.h"
#include <assimp/material.h>

using namespace std;

class aiMesh;
class aiScene;
class aiMaterial;
class aiNode;
class Model
{
public:
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader);
private:
    // model data
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);
    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma=true);
};