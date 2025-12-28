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
class Shader;
class aiNode;
class Model
{
public:
    Model(std::string path):shader_(nullptr)
    {
        loadModel(path);
    }
    Model(std::string path,Shader* shader) :shader_(shader)
    {
        loadModel(path);
    }
    ~Model();
    void Draw(Shader& shader);
    void Draw();
    Shader* getShader();

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
    Shader* shader_;
};