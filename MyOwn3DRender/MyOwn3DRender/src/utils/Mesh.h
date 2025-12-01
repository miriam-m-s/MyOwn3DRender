#pragma once
#include<vector>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <string>
using namespace std;
class Shader;
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
struct Texture {
    unsigned int id;
    string type;
    string path;  // we store the path of the texture to compare with other textures
};
class Mesh {
public:
    // mesh data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader& shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};