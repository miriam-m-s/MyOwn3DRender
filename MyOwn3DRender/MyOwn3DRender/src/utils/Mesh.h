#pragma once
#include<vector>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include"Texture.h"
#include <string>
#include <glm/glm.hpp>
using namespace std;
class Shader;
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec4 color;
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
    glm::mat4 model = glm::mat4(1.0f);
    void setupMesh();
};